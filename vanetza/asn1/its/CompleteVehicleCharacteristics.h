/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "IVI"
 * 	found in "IS_TS103301/ISO_TS_19321.asn"
 * 	`asn1c -fcompound-names -fincludes-quoted -no-gen-example`
 */

#ifndef	_CompleteVehicleCharacteristics_H_
#define	_CompleteVehicleCharacteristics_H_


#include "asn_application.h"

/* Including external dependencies */
#include "asn_SEQUENCE_OF.h"
#include "constr_SEQUENCE_OF.h"
#include "constr_SEQUENCE.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct TractorCharacteristics;
struct TrainCharacteristics;
struct TrailerCharacteristics;

/* CompleteVehicleCharacteristics */
typedef struct CompleteVehicleCharacteristics {
	struct TractorCharacteristics	*tractor	/* OPTIONAL */;
	struct CompleteVehicleCharacteristics__trailer {
		A_SEQUENCE_OF(struct TrailerCharacteristics) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *trailer;
	struct TrainCharacteristics	*train	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CompleteVehicleCharacteristics_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CompleteVehicleCharacteristics;
extern asn_SEQUENCE_specifics_t asn_SPC_CompleteVehicleCharacteristics_specs_1;
extern asn_TYPE_member_t asn_MBR_CompleteVehicleCharacteristics_1[3];

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "TractorCharacteristics.h"
#include "TrainCharacteristics.h"
#include "TrailerCharacteristics.h"

#endif	/* _CompleteVehicleCharacteristics_H_ */
#include "asn_internal.h"
