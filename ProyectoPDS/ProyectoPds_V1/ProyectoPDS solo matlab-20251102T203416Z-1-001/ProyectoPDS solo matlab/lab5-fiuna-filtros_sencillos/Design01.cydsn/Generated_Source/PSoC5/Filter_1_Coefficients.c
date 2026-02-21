#include "Filter_1.h"
#include "Filter_1_PVT.h"


/*******************************************************************************
* ChannelA filter coefficients.
* Filter Type is: Biquad
*******************************************************************************/

/* Renamed array for backward compatibility.
*  Should not be used in new designs.
*/
#define ChannelABiquadCoefficients Filter_1_ChannelABiquadCoefficients

/* Number of Biquad sections are: 10 */

const uint8 CYCODE Filter_1_ChannelABiquadCoefficients[Filter_1_BIQUAD_A_SIZE] = 
{
 /* Coefficients of Section 0 */
 0x82u, 0x06u, 0x0Au, 0x00u, /* Section(0)_A0, 0.156647205352783 */

 0xFDu, 0xF2u, 0xEBu, 0x00u, /* Section(0)_A1, -0.313294172286987 */

 0x82u, 0x06u, 0x0Au, 0x00u, /* Section(0)_A2, 0.156647205352783 */

 0x57u, 0x50u, 0x5Fu, 0x00u, /* Section(0)_B1, -1.48927855491638 */

 0x87u, 0x06u, 0xD2u, 0x00u, /* Section(0)_B2, 0.718351602554321 */

 /* Coefficients of Section 1 */
 0x0Cu, 0xBCu, 0x1Cu, 0x00u, /* Section(1)_A0, 0.448977470397949 */

 0xE7u, 0x87u, 0xC6u, 0x00u, /* Section(1)_A1, -0.897955179214478 */

 0x0Cu, 0xBCu, 0x1Cu, 0x00u, /* Section(1)_A2, 0.448977470397949 */

 0x43u, 0x97u, 0x5Cu, 0x00u, /* Section(1)_B1, -1.44673228263855 */

 0x75u, 0x9Eu, 0xD1u, 0x00u, /* Section(1)_B2, 0.724703550338745 */

 /* Coefficients of Section 2 */
 0x22u, 0x40u, 0x17u, 0x00u, /* Section(2)_A0, 0.363289356231689 */

 0xBDu, 0x7Fu, 0xD1u, 0x00u, /* Section(2)_A1, -0.7265784740448 */

 0x22u, 0x40u, 0x17u, 0x00u, /* Section(2)_A2, 0.363289356231689 */

 0x44u, 0xC9u, 0x5Bu, 0x00u, /* Section(2)_B1, -1.43415927886963 */

 0xE5u, 0x10u, 0xCFu, 0x00u, /* Section(2)_B2, 0.764593839645386 */

 /* Coefficients of Section 3 */
 0xABu, 0x79u, 0x0Fu, 0x00u, /* Section(3)_A0, 0.241801023483276 */

 0xAAu, 0x0Cu, 0xE1u, 0x00u, /* Section(3)_A1, -0.483602046966553 */

 0xABu, 0x79u, 0x0Fu, 0x00u, /* Section(3)_A2, 0.241801023483276 */

 0xA2u, 0x40u, 0x5Du, 0x00u, /* Section(3)_B1, -1.45706987380981 */

 0x53u, 0x62u, 0xCAu, 0x00u, /* Section(3)_B2, 0.837748765945435 */

 /* Coefficients of Section 4 */
 0x19u, 0x00u, 0x04u, 0x00u, /* Section(4)_A0, 0.0625059604644775 */

 0x32u, 0x00u, 0x08u, 0x00u, /* Section(4)_A1, 0.125011920928955 */

 0x19u, 0x00u, 0x04u, 0x00u, /* Section(4)_A2, 0.0625059604644775 */

 0x3Du, 0x5Au, 0x63u, 0x00u, /* Section(4)_B1, -1.55238270759583 */

 0x05u, 0x93u, 0xD0u, 0x00u, /* Section(4)_B2, 0.741026639938354 */

 /* Coefficients of Section 5 */
 0x1Du, 0xD3u, 0x03u, 0x00u, /* Section(5)_A0, 0.0597603321075439 */

 0x3Au, 0xA6u, 0x07u, 0x00u, /* Section(5)_A1, 0.119520664215088 */

 0x1Du, 0xD3u, 0x03u, 0x00u, /* Section(5)_A2, 0.0597603321075439 */

 0x6Au, 0xFFu, 0x67u, 0x00u, /* Section(5)_B1, -1.62496423721313 */

 0xE5u, 0xD1u, 0xCDu, 0x00u, /* Section(5)_B2, 0.784064054489136 */

 /* Coefficients of Section 6 */
 0x28u, 0x6Bu, 0x03u, 0x00u, /* Section(6)_A0, 0.0534152984619141 */

 0x50u, 0xD6u, 0x06u, 0x00u, /* Section(6)_A1, 0.106830596923828 */

 0x28u, 0x6Bu, 0x03u, 0x00u, /* Section(6)_A2, 0.0534152984619141 */

 0x09u, 0xB8u, 0x6Cu, 0x00u, /* Section(6)_B1, -1.69873261451721 */

 0xE1u, 0x50u, 0xCAu, 0x00u, /* Section(6)_B2, 0.838813543319702 */

 /* Coefficients of Section 7 */
 0xB1u, 0x88u, 0x02u, 0x00u, /* Section(7)_A0, 0.039592981338501 */

 0x62u, 0x11u, 0x05u, 0x00u, /* Section(7)_A1, 0.079185962677002 */

 0xB1u, 0x88u, 0x02u, 0x00u, /* Section(7)_A2, 0.039592981338501 */

 0x17u, 0x48u, 0x71u, 0x00u, /* Section(7)_B1, -1.77002501487732 */

 0x17u, 0x67u, 0xC6u, 0x00u, /* Section(7)_B2, 0.899957895278931 */

 /* Coefficients of Section 8 */
 0x30u, 0x46u, 0x02u, 0x00u, /* Section(8)_A0, 0.0355339050292969 */

 0x60u, 0x8Cu, 0x04u, 0x00u, /* Section(8)_A1, 0.0710678100585938 */

 0x30u, 0x46u, 0x02u, 0x00u, /* Section(8)_A2, 0.0355339050292969 */

 0xF5u, 0x1Fu, 0x61u, 0x00u, /* Section(8)_B1, -1.51757550239563 */

 0x61u, 0xD4u, 0xC3u, 0x00u, /* Section(8)_B2, 0.940162420272827 */

 /* Coefficients of Section 9 */
 0x8Fu, 0x0Au, 0x2Au, 0x00u, /* Section(9)_A0, 2.62757778167725 */

 0xE2u, 0xEAu, 0xABu, 0x00u, /* Section(9)_A1, -5.25515556335449 */

 0x8Fu, 0x0Au, 0x2Au, 0x00u, /* Section(9)_A2, 2.62757778167725 */

 0xB1u, 0xA5u, 0x75u, 0x00u, /* Section(9)_B1, -1.83823800086975 */

 0x62u, 0x34u, 0xC2u, 0x00u, /* Section(9)_B2, 0.965552806854248 */
};

