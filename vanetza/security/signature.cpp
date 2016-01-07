#include <boost/iostreams/stream.hpp>
#include <vanetza/common/byte_buffer_sink.hpp>
#include <vanetza/security/exception.hpp>
#include <vanetza/security/signature.hpp>
#include <cassert>

namespace vanetza
{
namespace security
{

PublicKeyAlgorithm get_type(const Signature& sig)
{
    struct Signature_visitor : public boost::static_visitor<PublicKeyAlgorithm>
    {
        PublicKeyAlgorithm operator()(const EcdsaSignature& sig)
        {
            return PublicKeyAlgorithm::Ecdsa_Nistp256_With_Sha256;
        }
    };
    Signature_visitor visit;
    return boost::apply_visitor(visit, sig);
}

size_t get_size(const EcdsaSignature& sig)
{
    size_t size = sig.s.size();
    size += get_size(sig.R);
    return size;
}

size_t get_size(const Signature& sig)
{
    size_t size = sizeof(PublicKeyAlgorithm);
    struct Signature_visitor : public boost::static_visitor<size_t>
    {
        size_t operator()(const EcdsaSignature& sig)
        {
            return get_size(sig);
        }
    };
    Signature_visitor visit;
    size += boost::apply_visitor(visit, sig);
    return size;
}

void serialize(OutputArchive& ar, const Signature& sig)
{
    struct signature_visitor : public boost::static_visitor<>
    {
        signature_visitor(OutputArchive& ar, PublicKeyAlgorithm algo) :
            m_archive(ar), m_algo(algo)
        {
        }

        void operator()(const EcdsaSignature& sig)
        {
            assert(field_size(m_algo) == sig.s.size());
            serialize(m_archive, sig.R, m_algo);
            for (auto& byte : sig.s) {
                m_archive << byte;
            }
        }

        OutputArchive& m_archive;
        PublicKeyAlgorithm m_algo;
    };

    PublicKeyAlgorithm algo = get_type(sig);
    serialize(ar, algo);
    signature_visitor visitor(ar, algo);
    boost::apply_visitor(visitor, sig);
}

size_t deserialize(InputArchive& ar, EcdsaSignature& sig, const PublicKeyAlgorithm& algo)
{
    EccPoint point;
    ByteBuffer buf;
    deserialize(ar, point, algo);
    for (size_t i = 0; i < field_size(algo); i++) {
        uint8_t byte;
        ar >> byte;
        buf.push_back(byte);
    }
    sig.R = point;
    sig.s = buf;
    return get_size(sig);
}

size_t deserialize(InputArchive& ar, Signature& sig)
{
    PublicKeyAlgorithm algo;
    size_t size = 0;
    deserialize(ar, algo);
    size += sizeof(algo);
    switch (algo) {
        case PublicKeyAlgorithm::Ecdsa_Nistp256_With_Sha256: {
            EcdsaSignature signature;
            size += deserialize(ar, signature, algo);
            sig = signature;
            break;
        }
        default:
            throw deserialization_error("Unknown PublicKeyAlgorithm");
    }
    return size;
}

ByteBuffer extract_signature_buffer(const Signature& sig)
{
    ByteBuffer buf;
    if (PublicKeyAlgorithm::Ecdsa_Nistp256_With_Sha256 == get_type(sig)) {
        EcdsaSignature signature = boost::get<EcdsaSignature>(sig);
        buf = convert_for_signing(signature.R);
        buf.insert(buf.end(), signature.s.begin(), signature.s.end());
    }
    return buf;
}

} // ns security
} // ns vanetza
