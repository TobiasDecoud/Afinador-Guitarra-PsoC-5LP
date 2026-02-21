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
 0x63u, 0x28u, 0x04u, 0x00u, /* Section(0)_A0, 0.0649650096893311 */

 0x3Bu, 0xAFu, 0xF7u, 0x00u, /* Section(0)_A1, -0.129929780960083 */

 0x63u, 0x28u, 0x04u, 0x00u, /* Section(0)_A2, 0.0649650096893311 */

 0xC2u, 0x49u, 0x78u, 0x00u, /* Section(0)_B1, -1.8795018196106 */

 0x33u, 0xC8u, 0xC3u, 0x00u, /* Section(0)_B2, 0.940905809402466 */

 /* Coefficients of Section 1 */
 0xB5u, 0xCFu, 0x0Du, 0x00u, /* Section(1)_A0, 0.215802431106567 */

 0x97u, 0x60u, 0xE4u, 0x00u, /* Section(1)_A1, -0.431604623794556 */

 0xB5u, 0xCFu, 0x0Du, 0x00u, /* Section(1)_A2, 0.215802431106567 */

 0x65u, 0x09u, 0x79u, 0x00u, /* Section(1)_B1, -1.89119839668274 */

 0xB0u, 0x4Cu, 0xC3u, 0x00u, /* Section(1)_B2, 0.948444366455078 */

 /* Coefficients of Section 2 */
 0x1Du, 0xC8u, 0x0Bu, 0x00u, /* Section(2)_A0, 0.184088945388794 */

 0xC6u, 0x6Fu, 0xE8u, 0x00u, /* Section(2)_A1, -0.368177890777588 */

 0x1Du, 0xC8u, 0x0Bu, 0x00u, /* Section(2)_A2, 0.184088945388794 */

 0x5Bu, 0xFBu, 0x79u, 0x00u, /* Section(2)_B1, -1.90596652030945 */

 0xFAu, 0x8Cu, 0xC2u, 0x00u, /* Section(2)_B2, 0.960145473480225 */

 /* Coefficients of Section 3 */
 0xE0u, 0x45u, 0x00u, 0x00u, /* Section(3)_A0, 0.00426483154296875 */

 0xC0u, 0x8Bu, 0x00u, 0x00u, /* Section(3)_A1, 0.0085296630859375 */

 0xE0u, 0x45u, 0x00u, 0x00u, /* Section(3)_A2, 0.00426483154296875 */

 0x57u, 0xD8u, 0x77u, 0x00u, /* Section(3)_B1, -1.87257933616638 */

 0xB0u, 0xECu, 0xC3u, 0x00u, /* Section(3)_B2, 0.938678741455078 */

 /* Coefficients of Section 4 */
 0x0Eu, 0x4Bu, 0x00u, 0x00u, /* Section(4)_A0, 0.00458097457885742 */

 0x1Bu, 0x96u, 0x00u, 0x00u, /* Section(4)_A1, 0.00916171073913574 */

 0x0Eu, 0x4Bu, 0x00u, 0x00u, /* Section(4)_A2, 0.00458097457885742 */

 0xEFu, 0xC3u, 0x77u, 0x00u, /* Section(4)_B1, -1.87133383750916 */

 0xE7u, 0xABu, 0xC3u, 0x00u, /* Section(4)_B2, 0.942632913589478 */

 /* Coefficients of Section 5 */
 0x1Bu, 0x74u, 0x0Au, 0x00u, /* Section(5)_A0, 0.163336515426636 */

 0xCAu, 0x17u, 0xEBu, 0x00u, /* Section(5)_A1, -0.326673030853271 */

 0x1Bu, 0x74u, 0x0Au, 0x00u, /* Section(5)_A2, 0.163336515426636 */

 0x58u, 0x0Cu, 0x7Bu, 0x00u, /* Section(5)_B1, -1.92262840270996 */

 0x3Du, 0x9Cu, 0xC1u, 0x00u, /* Section(5)_B2, 0.974838972091675 */

 /* Coefficients of Section 6 */
 0x98u, 0x57u, 0x00u, 0x00u, /* Section(6)_A0, 0.00534629821777344 */

 0x30u, 0xAFu, 0x00u, 0x00u, /* Section(6)_A1, 0.0106925964355469 */

 0x98u, 0x57u, 0x00u, 0x00u, /* Section(6)_A2, 0.00534629821777344 */

 0x22u, 0x1Du, 0x78u, 0x00u, /* Section(6)_B1, -1.87677812576294 */

 0xC7u, 0x02u, 0xC3u, 0x00u, /* Section(6)_B2, 0.952955484390259 */

 /* Coefficients of Section 7 */
 0xFFu, 0x51u, 0x00u, 0x00u, /* Section(7)_A0, 0.0050046443939209 */

 0xFDu, 0xA3u, 0x00u, 0x00u, /* Section(7)_A1, 0.0100090503692627 */

 0xFFu, 0x51u, 0x00u, 0x00u, /* Section(7)_A2, 0.0050046443939209 */

 0x23u, 0xE0u, 0x78u, 0x00u, /* Section(7)_B1, -1.88868021965027 */

 0xC4u, 0xFCu, 0xC1u, 0x00u, /* Section(7)_B2, 0.968947410583496 */

 /* Coefficients of Section 8 */
 0x71u, 0x68u, 0x08u, 0x00u, /* Section(8)_A0, 0.131374597549438 */

 0x1Du, 0x2Fu, 0xEFu, 0x00u, /* Section(8)_A1, -0.262749433517456 */

 0x71u, 0x68u, 0x08u, 0x00u, /* Section(8)_A2, 0.131374597549438 */

 0xB4u, 0x27u, 0x7Cu, 0x00u, /* Section(8)_B1, -1.93992328643799 */

 0x5Eu, 0x8Du, 0xC0u, 0x00u, /* Section(8)_B2, 0.991371631622314 */

 /* Coefficients of Section 9 */
 0x83u, 0xE6u, 0x01u, 0x00u, /* Section(9)_A0, 0.0296943187713623 */

 0x06u, 0xCDu, 0x03u, 0x00u, /* Section(9)_A1, 0.0593886375427246 */

 0x83u, 0xE6u, 0x01u, 0x00u, /* Section(9)_A2, 0.0296943187713623 */

 0xC2u, 0xFDu, 0x79u, 0x00u, /* Section(9)_B1, -1.9061131477356 */

 0xEAu, 0xB2u, 0xC0u, 0x00u, /* Section(9)_B2, 0.98907995223999 */
};

