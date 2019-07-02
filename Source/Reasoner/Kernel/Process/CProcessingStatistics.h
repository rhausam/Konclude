/*
 *		Copyright (C) 2011, 2012, 2013 by the Konclude Developer Team
 *
 *		This file is part of the reasoning system Konclude.
 *		For details and support, see <http://konclude.com/>.
 *
 *		Konclude is released as free software, i.e., you can redistribute it and/or modify
 *		it under the terms of version 3 of the GNU Lesser General Public License (LGPL3) as
 *		published by the Free Software Foundation.
 *
 *		You should have received a copy of the GNU Lesser General Public License
 *		along with Konclude. If not, see <http://www.gnu.org/licenses/>.
 *
 *		Konclude is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For more
 *		details see GNU Lesser General Public License.
 *
 */

#ifndef KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSINGSTATISTICS_H
#define KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSINGSTATISTICS_H

// Libraries includes
#include <QMutexLocker>
#include <QMutex>
#include <QVector>
#include <QList>
#include <QHash>

// Namespace includes
#include "ProcessSettings.h"
#include "CProcessingStatisticDescription.h"
#include "CProcessingStatisticRegistrator.h"


// Other includes


// Logger includes
#include "Logger/CLogger.h"



namespace Konclude {

	namespace Reasoner {

		namespace Kernel {

			namespace Process {

				/*! 
				 *
				 *		\class		CProcessingStatistics
				 *		\author		Andreas Steigmiller
				 *		\version	0.1
				 *		\brief		TODO
				 *
				 */
				class CProcessingStatistics : public CProcessingStatisticRegistrator {
					// public methods
					public:
						const static cint64 statisticVectorSize = 220;
						static cint64 statisticCount;


						const static CProcessingStatisticDescription
							statINDIVIDUALNODESWITCHCOUNT,
							statTASKPROCESSCHANGECOUNT,
							statTASKPROCESSPAUSECOUNT,
							statTASKCREATIONCOUNT,
							statROOTTASKSATISFIABLECOUNT,
							statROOTTASKUNSATISFIABLECOUNT,


							// statistics for completion graph reuse caching
							statCOMPLETIONGRAPHREUSECACHERETRIEVALCOUNT,
							statCOMPLETIONGRAPHREUSECACHERETRIEVALSUCCESSCOUNT,
							statCOMPLETIONGRAPHREUSECACHERETRIEVALFAILEDCOUNT,
							statCOMPLETIONGRAPHREUSECACHEREUSINGDETCOUNT,
							statCOMPLETIONGRAPHREUSECACHEREUSINGNONDETCOUNT,


							// statistics for unsatisfiable cache retrieval
							statUNSATCACHERETRIEVALCOUNT,
							statUNSATCACHERETRIEVALHASHCOUNT,
							statUNSATCACHERETRIEVALHASHSUCCESSCOUNT,
							statUNSATCACHERETRIEVALHASHFAILEDCOUNT,
							statUNSATCACHERETRIEVALPRECHECKCOUNT,
							statUNSATCACHERETRIEVALPRECHECKSUCCESSCOUNT,
							statUNSATCACHERETRIEVALPRECHECKDIRECTFAILEDCOUNT,
							statUNSATCACHERETRIEVALPRECHECKINSUFFICIENTCOUNT,
							statUNSATCACHEUSEDCOUNT,

							// statistics for unsatisfiable caching
							statUNSATCACHEWRITINGREQUSTCOUNT,
							statUNSATCACHEWRITINGREQUSTDIFFNODELEVELFAILEDCOUNT,
							statUNSATCACHEWRITINGREQUSTDIFFNOMINALFAILEDCOUNT,
							statUNSATCACHEWRITINGREQUSTATOMICCLASHFAILEDCOUNT,
							statUNSATCACHEWRITINGTRYINGCOUNT,
							statUNSATCACHEWRITINGALREADYCACHEDTESTCOUNT,
							statUNSATCACHEWRITINGALREADYCACHEDTESTPRECHECKFAILEDCOUNT,
							statUNSATCACHEWRITINGALREADYCACHEDTESTHASHFAILEDCOUNT,
							statUNSATCACHEWRITINGALREADYCACHEDCOUNT,
							statUNSATCACHEWRITINGCOUNT,

							// statistics for satisfiable/expansion caching retrieval
							statSATEXPCACHERETRIEVALCOUNT,
							statSATEXPCACHERETRIEVALSUCCESSCOUNT,
							statSATEXPCACHERETRIEVALFOUNDEXPANSIONCOUNT,
							statSATEXPCACHERETRIEVALFOUNDSATISFIABLECOUNT,
							statSATEXPCACHERETRIEVALCOMPATIBILITYTESTCOUNT,
							statSATEXPCACHERETRIEVALCOMPATIBLESATCOUNT,
							statSATEXPCACHERETRIEVALINCOMPATIBLESATCOUNT,
							statSATEXPCACHERETRIEVALEXPANDEDCONCEPTSCOUNT,

							// statistics for satisfiable caching
							statSATEXPCACHEWRITINGREQUESTCOUNT,
							statSATEXPCACHESATISFIABLEWRITINGREQUESTCOUNT,
							statSATEXPCACHEEXPANSIONWRITINGREQUESTCOUNT,
							statSATEXPCACHEWRITINGSIGNATUREALREADYCACHEDCOUNT,
							statSATEXPCACHEWRITINGSIGNATUREALREADYCACHEDINCOMPATIBLECOUNT,
							statSATEXPCACHEWRITINGSIGNATUREALREADYCACHEDSATADDEDCOUNT,
							statSATEXPCACHEWRITINGSIGNATURESATISFIABLECOUNT,
							statSATEXPCACHEWRITINGSIGNATUREEXPANSIONCOUNT,
							statSATEXPCACHEWRITINGSIGSATCRITICALBRANCHCONCEPTADDEDCOUNT,
							statSATEXPCACHEWRITINGEXPANSIONDELAYEDCOUNT,
							statSATEXPCACHEWRITINGERRORCOUNT,

							// statistics for satisfiable cached concept absorption
							statSATCACHEDABSORBEDDISJUNCTIONCONCEPTSCOUNT,
							statSATCACHEDABSORBEDMERGINGCONCEPTSCOUNT,
							statSATCACHEDABSORBEDGENERATINGCONCEPTSCOUNT,

							// statistics for signature mirroring blocking
							statSIGNATURESAVINGCOUNT,
							statSIGNATUREMIRRORINGBLOCKINGREFRESHCOUNT,
							statSIGNATUREMIRRORINGBLOCKINGREFRESHLOSEDCOUNT,
							statSIGNATUREMIRRORINGBLOCKINGSEARCHCOUNT,
							statSIGNATUREMIRRORINGBLOCKINGESTABLISHCOUNT,
							statSIGNATUREMIRRORINGBLOCKINGADDFOLLOWINGCOUNT,
							statSIGNATUREMIRRORINGBLOCKINGREMOVEFOLLOWINGCOUNT,
							statSIGNATUREMIRRORINGBLOCKINGCONCEPTEXPANSIONCOUNT,
							statSIGNATUREMIRRORINGBLOCKINGCANDIDATEREGARDEDCOUNT,
							statSIGNATUREMIRRORINGBLOCKINGCANDIDATEREGARDEDINVALIDCOUNT,
							statSIGNATUREMIRRORINGBLOCKINGCANDIDATEREGARDEDINCOMPATIBLECOUNT,

							// statistics for back jumping
							statTASKBACKJUMPINGCOUNT,
							statTASKAWAYBACKJUMPINGCOUNT,
							statTASKROOTBACKJUMPINGCOUNT,

							// statistics for backtracking
							statCLASHEDBACKTRACKINGSTARTCOUNT,
							statCLASHEDBACKTRACKINGSTEPSCOUNT,
							statCLASHEDBACKTRACKINGPREVNODELEVELCOUNT,
							statCLASHEDBACKTRACKINGPREVNODELEVELBRANCHCOUNT,
							statCLASHEDBACKTRACKINGCURRNODELEVELBRANCHCOUNT,
							statCLASHEDBACKTRACKINGINDEPENDENTCOUNT,
							statCLASHEDBACKTRACKGNONDETERMINISTICCOUNT,
							statCLASHEDBACKTRACKGNONDETALREADYCLASHFAILEDCOUNT,
							statCLASHEDBACKTRACKGNONDETCLOSEDCOUNT,
							statCLASHEDBACKTRACKEDDESCRIPTORSCOUNT,

							// statistics for counting rule applications
							statRULEAPPLICATIONCOUNT,
							statAUTOMATERULEAPPLICATIONCOUNT,
							statANDRULEAPPLICATIONCOUNT,
							statSOMERULEAPPLICATIONCOUNT,
							statFUNCTIONALRULEAPPLICATIONCOUNT,
							statATMOSTRULEAPPLICATIONCOUNT,
							statATLEASTRULEAPPLICATIONCOUNT,
							statORRULEAPPLICATIONCOUNT,
							statSELFRULEAPPLICATIONCOUNT,
							statALLRULEAPPLICATIONCOUNT,
							statNOMINALRULEAPPLICATIONCOUNT,
							statVALUERULEAPPLICATIONCOUNT,
							statIMPLICATIONRULEAPPLICATIONCOUNT,

							// statistics for merging and qualifying
							statINDINODEMERGEINITCOUNT,
							statINDINODEQUALIFYCHOOCECOUNT,
							statINDINODEMERGECOUNT,
							statINDINODEMERGENEWNOMINALINITCOUNT,
							statINDINODESMERGEABLETESTCOUNT,
							statINDINODESMERGEABLEDISJOINTROLETESTCOUNT,
							statINDINODESMERGEABLECONCEPTSETTESTCOUNT,
							statINDINODESMERGEABLEDISTINCTTESTCOUNT,
							statINDINODESMERGEABLETESTSUCCESSCOUNT,
							statINDINODEMERGECONCEPTSADDCOUNT,
							statINDINODEMERGELINKSADDCOUNT,
							statINDINODEMERGEDISTINCTADDCOUNT,

							// statistics for implication
							statIMPLICATIONTRIGGERINGCOUNT,
							statIMPLICATIONEXECUTINGCOUNT,

							// statistics for nominals
							statINDINODENOMINALMERGECOUNT,
							statINDINODENOMINALDISTINCTCOUNT,
							statINDINODENOMINALCORRECTIDCOUNT,

							// statistics for ALL and Automate rules
							statALLROLERESTRICTIONCOUNT,
							statAUTOMATETRANSACTIONCOUNT,
							statAUTOMATESTATECOUNT,
							statAUTOMATEINITCOUNT,

							// statistics for successor individual node creation
							statSUCCESSORINDINODECREATIONCOUNT,
							statDISTINCTSUCCESSORINDINODECREATIONCOUNT,
							statSEPARATESUCCESSORINDINODECREATIONCOUNT,
							statNOMINALSUCCESSORINDINODECREATIONCOUNT,

							// statistics for task creation
							statTASKDISTINCTMERGEBRANCHCREATIONCOUNT,
							statTASKINDINODEMERGEBRANCHCREATIONCOUNT,
							statTASKQUALIFYCHOOSEBRANCHCREATIONCOUNT,
							statTASKORBRANCHCREATIONCOUNT,

							// statistics for OR processing
							statOREXECUTIONINITCOUNT,
							statOREXECUTIONCONTINUECOUNT,
							statORINSTANTCONTAINSEXECUTEDCOUNT,
							statORTRIGGEREDDELAYEDEXECUTEDCOUNT,
							statORTRIGGERSEARCHCOUNT,
							statORQUEUEDDELAYEDEXECUTEDCOUNT,
							statOREXECUTIONCOUNT,
							statORSINGLEBRANCHCOUNT,
							statORMULTIPLEBRANCHCOUNT,
							statORREPLACEMENTIMPLICATIONCOUNT,

							// statistics for triggers
							statCONCEPTTRIGGERINSTALLCOUNT,
							statROLETRIGGERINSTALLCOUNT,

							// statistics for concept set tests
							statLABELCONCEPTSUBSETTESTCOUNT,
							statLABELCONCEPTEQUALSETTESTCOUNT,
							statLABELCONCEPTPAIRWISEEQUALSETTESTCOUNT,

							// statistics for blocking
							statOPTIMIZEDBLOCKINGB2AUTOMATETRANSACTIONTESTCOUNT,
							statOPTIMIZEDBLOCKINGTESTCOUNT,
							statSUBSETBLOCKINGTESTCOUNT,
							statEQUALSETBLOCKINGTESTCOUNT,
							statPAIRWISEEQUALSETBLOCKINGTESTCOUNT,
							statINDINODEBLOCKINGTESTCOUNT,
							statDETECTINDINODEBLOCKINGSTATUSCOUNT,
							statDETECTANCINDINODEBLOCKINGSTATUSCOUNT,
							statSUCCESSBLOCKINGSTATUSDETECTIONCOUNT,
							statFAILEDBLOCKINGSTATUSDETECTIONCOUNT,

							statCONTINUEBLOCKINGTESTCOUNT,
							statANCESTORBLOCKINGSEARCHINDINODECOUNT,
							statANYWHEREBLOCKINGSEARCHINDINODECOUNT,
							statANYWHEREBLOCKINGCANDIDATEHASHSEARCHINDINODECOUNT,
							statANYWHEREBLOCKINGCANDIDATEHASHUDATECOUNT,
							statANYWHEREBLOCKINGCANDIDATEHASHUDATEADDCOUNT,
							statANYWHEREBLOCKINGCANDIDATEHASHUDATEREMOVECOUNT,
							statANYWHERECORECONCEPTBLOCKINGCANDIDATEHASHSEARCHINDINODECOUNT,

							// statistics for blocking test calls
							statINDINODEEXPANSIONBLOCKINGTESTCOUNT,
							statINDINODEPROCESSINGBLOCKINGTESTCOUNT,

							// statistics for creation
							statLINKSCREATIONCOUNT,
							statDISTINCTCREATIONCOUNT,
							statCONCEPTSADDEDINDINODELABELSETCOUNT,
							statCONCEPTSADDEDINDINODEPROCESSINGQUEUECOUNT,
							statINDINODESADDEDPROCESSINGQUEUECOUNT,
							statINSERTCONCEPTREAPPLICATIONSCOUNT,
							statREAPPLIEDCONCEPTSCOUNT,
							statCORECONCEPTSADDEDINDINODELABELSETCOUNT,

							// statistics for creation
							statINDINODEUPDATELOADCOUNT,
							statINDINODELOCALIZEDLOADCOUNT,



							// statistics for binding candidate propagation
							statPBINDVARIABLEBINDCOUNT,
							statPBINDPROPAGATEDCOUNT,
							statPBINDPROPAGATEDFRESHCOUNT,
							statPBINDPROPAGATEDINITIALCOUNT,
							statPBINDGROUNDINGCOUNT,
							statPBINDCYCLEPROPAGATECOUNT,
							statPBINDCYCLEREAPPLYINSTALLCOUNT,
							statPBINDRULEANDAPPLICATIONCOUNT,
							statPBINDRULEALLAPPLICATIONCOUNT,
							statPBINDRULEGROUNDINGAPPLICATIONCOUNT,
							statPBINDRULECYCLEAPPLICATIONCOUNT,
							statPBINDRULEIMPLICATIONAPPLICATIONCOUNT,
							statPBINDRULEBINDNAPPLICATIONCOUNT,
							statPBINDREAPPLICATIONCOUNT,


							// statistics for variable binding propagation
							statVARBINDVARIABLEBINDCOUNT,
							statVARBINDPROPAGATEDCOUNT,
							statVARBINDPROPAGATEDFRESHCOUNT,
							statVARBINDPROPAGATEDINITIALCOUNT,
							statVARBINDRULEANDAPPLICATIONCOUNT,
							statVARBINDRULEALLAPPLICATIONCOUNT,
							statVARBINDRULEBINDAPPLICATIONCOUNT,
							statVARBINDRULEGROUNDINGAPPLICATIONCOUNT,
							statVARBINDRULEIMPLICATIONAPPLICATIONCOUNT,
							statVARBINDRULEJOINAPPLICATIONCOUNT,
							statVARBINDJOINTRIGGERINSTALLCOUNT,
							statVARBINDJOINTRIGGEREXECUTECOUNT,
							statVARBINDJOINCOMBINECOUNT,
							statVARBINDJOINCREATENEWCOUNT,
							statVARBINDGROUNDINGCOUNT,



							// statistics for timings
							statTIMEBACKTRACING,
							statTIMEUNSATCACHERETRIVAL,
							statTIMECOMPLETIONGRAPHREUSERETRIVAL,


							// statistics for analysing
							statANALYSESUBSUMERCOUNT,
							statANALYSEPOSSIBLESUBSUMPTIONSCOUNT,
							statANALYSEPSEUDOMODELCOUNT,
							statANALYSECANDIDATECONCEPTSATURATEDALTERNATIVETESTCOUNT,
							statANALYSECANDIDATECONCEPTSATURATEDALTERNATIVEMERGINGCOUNT,
							statANALYSECANDIDATECONCEPTSATURATEDCONCEPTSETMERGINGCOUNT,
							statANALYSECANDIDATECONCEPTSATURATEDROLESUCCESSORMERGINGCOUNT,
							statANALYSECANDIDATECONCEPTSATURATEDALTERNATIVEFOUNDCOUNT,
							statANALYSECANDIDATECONCEPTSATURATEDROLESUCCESSOREXTENDEDMERGINGCOUNT,

							// statistics for saturation caching
							statSATURATIONCACHECONCEPTEXPANSIONCOUNT,
							statSATURATIONCACHEESTABLISHCOUNT,
							statSATURATIONCACHELOSECOUNT,


							// statistics for saturation
							statSATURATIONCRITICALTESTCOUNT,
							statSATURATIONCRITICALALLCOUNT,
							statSATURATIONCRITICALORCOUNT,
							statSATURATIONCRITICALATMOSTCOUNT,
							statSATURATIONCRITICALADDCOUNT

							;



						static CProcessingStatistics* getProcessingStatistics();

						static cint64 getIndexFromProcessingStatisticDescriptionName(const QString& statDesName);


						QList<CProcessingStatisticDescription*>* getProcessingStatisticDescriptionList();

						virtual CProcessingStatisticRegistrator* registerProcessingStatisticDescription(CProcessingStatisticDescription* statDesc);


					// protected methods
					protected:
						//! Constructor
						CProcessingStatistics();

						//! Destructor
						virtual ~CProcessingStatistics();




					// protected variables
					protected:
						static CProcessingStatistics* mInstance;
						static QMutex mInstanceMutex;

						QVector<CProcessingStatisticDescription*> mStatDescVec;
						QList<CProcessingStatisticDescription*> mStatDescList;
						QHash<QString,CProcessingStatisticDescription*> mStatDescHash;

					// private methods
					private:

					// private variables
					private:

				};

			}; // end namespace Process

		}; // end namespace Kernel

	}; // end namespace Reasoner

}; // end namespace Konclude

#endif // KONCLUDE_REASONER_KERNEL_PROCESS_CPROCESSINGSTATISTICS_H
