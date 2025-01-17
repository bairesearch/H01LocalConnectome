/*******************************************************************************
 *
 * File Name: LCindexedCSVdatabasePreprocess.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2023 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: LCLocalConnectome
 * Requirements: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Compilation: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Usage: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Description: LC indexed CSV database preprocess - 
 * /
 *******************************************************************************/


#include "LCindexedCSVdatabasePreprocess.hpp"

#ifdef INDEXED_CSV_DATABASE_PREPROCESS

bool LCindexedCSVdatabasePreprocessClass::preprocess(const int preprocessMode, const string local_connectome_folder_base)
{
	bool result = true;
	
	#ifdef INDEXED_CSV_DATABASE_LDC
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
	if(preprocessMode == PREPROCESS_MODE_INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET)
	{
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS
		generateLocalConnectomeConnectionsDatasetFromMatrix(local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS);
		#endif
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_NEURONS_DATASET
	else if(preprocessMode == PREPROCESS_MODE_INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_NEURONS_DATASET)
	{
		generateLocalConnectomeNeuronsDatasetFromSkeletons(local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME);
	}
	#endif
	#elif defined INDEXED_CSV_DATABASE_ADC
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
	if(preprocessMode == PREPROCESS_MODE_INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET)
	{
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS
		generateLocalConnectomeConnectionsDatasetFromADCconnectionsFile(local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS);
		#endif
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_EM_IMAGES
		generateLocalConnectomeConnectionsDatasetFromADCconnectionsFile(local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES);
		#endif
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_NEURONS_DATASET
	else if(preprocessMode == PREPROCESS_MODE_INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_NEURONS_DATASET)
	{
		generateLocalConnectomeNeuronsDatasetFromADCneuronsFile(local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME);
	}
	#endif	
	#elif defined INDEXED_CSV_DATABASE_CEC
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET
	if(preprocessMode == PREPROCESS_MODE_INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET)
	{
		#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_EM_IMAGES
		generateLocalConnectomeConnectionsDatasetFromCECconnectionsFile(local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES);
		#endif
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_NEURONS_DATASET
	else if(preprocessMode == PREPROCESS_MODE_INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_NEURONS_DATASET)
	{
		generateLocalConnectomeNeuronsDatasetFromCECneuronsFile(local_connectome_folder_base, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME);
	}
	#endif	
	#endif
	else
	{
		cerr << "LCindexedCSVdatabasePreprocessClass::preprocess error: preprocessMode unknown: " << preprocessMode << endl;
		exit(EXIT_ERROR);
	}
	
	return result;
}


#ifdef INDEXED_CSV_DATABASE_CEC
void LCindexedCSVdatabasePreprocessClass::generateLocalConnectomeNeuronsDatasetFromCECneuronsFile(const string local_connectome_folder_base, const string neuronDatasetFileNameWrite)
{	
	LCindexedCSVdatabaseOperations.prepareLocalConnectomeDatasetRead(local_connectome_folder_base);
	
	vector<vector<string>> CECdistancesData;
	int CECdistancesDataSize;
	SHAREDvars.getLinesFromFileCSV(INDEXED_CSV_DATABASE_PREPROCESS_CEC_DISTANCES_DATAFILE_NAME, &CECdistancesData, &CECdistancesDataSize, CSV_DELIMITER_CHAR, true);
	
	string neuronDatasetDataString = string(LOCAL_CONNECTOME_DATASET_NEURONS_HEADER) + STRING_NEWLINE;
	for(int neuronIndex=0; neuronIndex<CECdistancesData.size(); neuronIndex++)
	{
		vector<string>* CECneuron = &(CECdistancesData[neuronIndex]);
		string neuronID = SHAREDvars.convertIntToString(neuronIndex);
		string neuronType = (*CECneuron)[INDEXED_CSV_DATABASE_PREPROCESS_CEC_DISTANCES_DATAFILE_FIELD_INDEX_NEURON];	//CHECKTHIS - currently store neuronType as alphabetical neuron name (the first few letters of which indicate its type)
		string excitationTypeString = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN);	//excitation types are currently taken from individual synapses
		
		//id,x,y,z,type,excitation_type
		string neuronDatasetLine = neuronID + CSV_DELIMITER_CHAR + (*CECneuron)[INDEXED_CSV_DATABASE_PREPROCESS_CEC_DISTANCES_DATAFILE_FIELD_INDEX_X] + CSV_DELIMITER_CHAR + (*CECneuron)[INDEXED_CSV_DATABASE_PREPROCESS_CEC_DISTANCES_DATAFILE_FIELD_INDEX_Y] + CSV_DELIMITER_CHAR + (*CECneuron)[INDEXED_CSV_DATABASE_PREPROCESS_CEC_DISTANCES_DATAFILE_FIELD_INDEX_Z] + CSV_DELIMITER_CHAR + neuronType + CSV_DELIMITER_CHAR + excitationTypeString; 	//NOTREQUIRED (automatically generated); CSV_DELIMITER_CHAR + layerString
		neuronDatasetDataString += neuronDatasetLine + STRING_NEWLINE;
	}
	
	bool write = true;
	if(write)
	{
		bool appendToFile = false;
		ofstream writeFileObject = LCindexedCSVdatabaseOperations.prepareLocalConnectomeDatasetWrite(write, appendToFile, neuronDatasetFileNameWrite);
		SHAREDvars.writeStringToFileObject(neuronDatasetDataString, &writeFileObject);
		LCindexedCSVdatabaseOperations.finaliseLocalConnectomeDatasetWrite(write, &writeFileObject);
	}
}

void LCindexedCSVdatabasePreprocessClass::generateLocalConnectomeConnectionsDatasetFromCECconnectionsFile(const string local_connectome_folder_base, const string connectionDatasetFileNameWrite, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
{
	LCindexedCSVdatabaseOperations.prepareLocalConnectomeDatasetRead(local_connectome_folder_base);
	
	vector<vector<string>> CECconnectomeData;
	int CECconnectomeDataSize;
	SHAREDvars.getLinesFromFileCSV(INDEXED_CSV_DATABASE_PREPROCESS_CEC_CONNECTOME_DATAFILE_NAME, &CECconnectomeData, &CECconnectomeDataSize, CSV_DELIMITER_CHAR, true);
	
	map<string, string> neuronIDMap = calculateNeuronIDMap(local_connectome_folder_base);
	
	string neuronDatasetDataString = string(LOCAL_CONNECTOME_DATASET_CONNECTIONS_HEADER) + STRING_NEWLINE;
	for(int connectionIndex=0; connectionIndex<CECconnectomeData.size(); connectionIndex++)
	{
		vector<string>* CECconnection = &(CECconnectomeData[connectionIndex]);
		string preNeuron = (*CECconnection)[INDEXED_CSV_DATABASE_PREPROCESS_CEC_CONNECTOME_DATAFILE_FIELD_INDEX_SOURCENEURON];
		string postNeuron = (*CECconnection)[INDEXED_CSV_DATABASE_PREPROCESS_CEC_CONNECTOME_DATAFILE_FIELD_INDEX_TARGETNEURON];
		string preID = neuronIDMap[preNeuron];
		string postID = neuronIDMap[postNeuron];
		string preClassLabel = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_UNKNOWN;
		string postClassLabel = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_UNKNOWN;
		string connectionSynNumFloatString = (*CECconnection)[INDEXED_CSV_DATABASE_PREPROCESS_CEC_CONNECTOME_DATAFILE_FIELD_INDEX_NUMBEROFCONNECTIONS];
		float connectionSynNumFloat = SHAREDvars.convertStringToFloat(connectionSynNumFloatString);
		int connectionSynNum = int(connectionSynNumFloat);
		string connectionSynNumString = SHAREDvars.convertIntToString(connectionSynNum);
		string excitationTypeAcronym = (*CECconnection)[INDEXED_CSV_DATABASE_PREPROCESS_CEC_CONNECTOME_DATAFILE_FIELD_INDEX_EXCITATIONTYPE];
		string excitationTypeString;
		if(excitationTypeAcronym == INDEXED_CSV_DATABASE_PREPROCESS_CEC_CONNECTOME_DATAFILE_FIELD_INDEX_EXCITATIONTYPE_EXCITATORY)
		{
			excitationTypeString = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY);
		}
		else if(excitationTypeAcronym == INDEXED_CSV_DATABASE_PREPROCESS_CEC_CONNECTOME_DATAFILE_FIELD_INDEX_EXCITATIONTYPE_INHIBITORY)
		{
			excitationTypeString = SHAREDvars.convertIntToString(LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY);
		}
		else
		{
			cerr << "LCindexedCSVdatabasePreprocessClass::generateLocalConnectomeConnectionsDatasetFromCECconnectionsFile error: excitationTypeAcronym unknown; excitationTypeAcronym = " << excitationTypeAcronym << endl;
			exit(EXIT_ERROR);
		}

		//pre_id, post_id, pre_class_label, post_class_label, syn_num, excitation_type
		string neuronDatasetLine = preID + CSV_DELIMITER_CHAR + postID + CSV_DELIMITER_CHAR + preClassLabel + CSV_DELIMITER_CHAR + postClassLabel + CSV_DELIMITER_CHAR + connectionSynNumString + CSV_DELIMITER_CHAR + excitationTypeString;
		neuronDatasetDataString += neuronDatasetLine + STRING_NEWLINE;
	}

	bool write = true;
	if(write)
	{
		bool appendToFile = false;
		ofstream writeFileObject = LCindexedCSVdatabaseOperations.prepareLocalConnectomeDatasetWrite(write, appendToFile, connectionDatasetFileNameWrite);
		SHAREDvars.writeStringToFileObject(neuronDatasetDataString, &writeFileObject);
		LCindexedCSVdatabaseOperations.finaliseLocalConnectomeDatasetWrite(write, &writeFileObject);
	}
}
#endif
	
#ifdef INDEXED_CSV_DATABASE_ADC
void LCindexedCSVdatabasePreprocessClass::generateLocalConnectomeNeuronsDatasetFromADCneuronsFile(const string local_connectome_folder_base, const string neuronDatasetFileNameWrite)
{	
	LCindexedCSVdatabaseOperations.prepareLocalConnectomeDatasetRead(local_connectome_folder_base);
	
	vector<vector<string>> ADCneuronsData;
	int ADCneuronsDataSize;
	SHAREDvars.getLinesFromFileCSV(INDEXED_CSV_DATABASE_PREPROCESS_ADC_NEURONS_DATAFILE_NAME, &ADCneuronsData, &ADCneuronsDataSize, CSV_DELIMITER_CHAR, true);
	
	map<string, string> neuronExcitationTypeMap = calculateNeuronExcitationTypeMap(local_connectome_folder_base, INDEXED_CSV_DATABASE_PREPROCESS_ADC_NEUROTRANSMITTER_TYPES_FILE_NAME);
	map<string, string> neuronCoordinatesMap = calculateNeuronCoordinatesTypeMap(local_connectome_folder_base);

	string neuronDatasetDataString = string(LOCAL_CONNECTOME_DATASET_NEURONS_HEADER) + STRING_NEWLINE;
	for(int neuronIndex=0; neuronIndex<ADCneuronsData.size(); neuronIndex++)
	{
		vector<string>* ADCneuron = &(ADCneuronsData[neuronIndex]);
		string neuronID = (*ADCneuron)[INDEXED_CSV_DATABASE_PREPROCESS_ADC_NEURONS_DATAFILE_FIELD_INDEX_ID];
		string neurotransmitterType = (*ADCneuron)[INDEXED_CSV_DATABASE_PREPROCESS_ADC_NEURONS_DATAFILE_FIELD_INDEX_NT_TYPE];
		string excitationTypeString = "";
		if(neuronExcitationTypeMap.count(neurotransmitterType) != 0)
		{
			excitationTypeString = SHAREDvars.convertIntToString(getNeuronExcitationTypeFromType(neurotransmitterType, &neuronExcitationTypeMap));
		}
		else
		{
			//cerr << "LCindexedCSVdatabasePreprocessClass::generateLocalConnectomeNeuronsDatasetFromADCneuronsFile error: (neuronExcitationTypeMap.count(neurotransmitterType) != 0); neurotransmitterType = " << neurotransmitterType << endl;
			excitationTypeString = SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_CONNECTIONS_EXCITATION_TYPE_UNKNOWN);
		}
		string neuronType = (*ADCneuron)[INDEXED_CSV_DATABASE_PREPROCESS_ADC_NEURONS_DATAFILE_FIELD_INDEX_GROUP];	//layer	//CHECKTHIS
		string positionVectorString = neuronCoordinatesMap[neuronID];
		vector<string> positionVector = parseADCcoordinatesPositionVectorString(positionVectorString);

		//id,x,y,z,type,excitation_type
		string neuronDatasetLine = neuronID + CSV_DELIMITER_CHAR + positionVector[0] + CSV_DELIMITER_CHAR + positionVector[1] + CSV_DELIMITER_CHAR + positionVector[2] + CSV_DELIMITER_CHAR + neuronType + CSV_DELIMITER_CHAR + excitationTypeString; 	//NOTREQUIRED (automatically generated); CSV_DELIMITER_CHAR + layerString
		neuronDatasetDataString += neuronDatasetLine + STRING_NEWLINE;
	}
	
	bool write = true;
	if(write)
	{
		bool appendToFile = false;
		ofstream writeFileObject = LCindexedCSVdatabaseOperations.prepareLocalConnectomeDatasetWrite(write, appendToFile, neuronDatasetFileNameWrite);
		SHAREDvars.writeStringToFileObject(neuronDatasetDataString, &writeFileObject);
		LCindexedCSVdatabaseOperations.finaliseLocalConnectomeDatasetWrite(write, &writeFileObject);
	}
}

vector<string> LCindexedCSVdatabasePreprocessClass::parseADCcoordinatesPositionVectorString(const string positionVectorString)
{	
	string positionVectorStringFormatted = positionVectorString.substr(1, positionVectorString.size()-2);	//remove [ and ] characters
	bool ignoreRepeatedDelimiters = true;
	vector<string> positionVector = SHAREDvars.getVectorFromListString(positionVectorStringFormatted, CHAR_SPACE, ignoreRepeatedDelimiters);
	return positionVector;
}

void LCindexedCSVdatabasePreprocessClass::generateLocalConnectomeConnectionsDatasetFromADCconnectionsFile(const string local_connectome_folder_base, const string connectionDatasetFileNameWrite, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
{
	LCindexedCSVdatabaseOperations.prepareLocalConnectomeDatasetRead(local_connectome_folder_base);
	
	vector<vector<string>> ADCconnectionsData;
	int ADCconnectionsDataSize;
	SHAREDvars.getLinesFromFileCSV(INDEXED_CSV_DATABASE_PREPROCESS_ADC_CONNECTIONS_DATAFILE_NAME, &ADCconnectionsData, &ADCconnectionsDataSize, CSV_DELIMITER_CHAR, true);
	
	map<string, string> neuronExcitationTypeMap = calculateNeuronExcitationTypeMap(local_connectome_folder_base, INDEXED_CSV_DATABASE_PREPROCESS_ADC_NEUROTRANSMITTER_TYPES_FILE_NAME);
	map<string, string> neuronNeurotransmitterTypeMap = calculateNeuronNeurotransmitterTypeMap(local_connectome_folder_base);
	
	string neuronDatasetDataString = string(LOCAL_CONNECTOME_DATASET_CONNECTIONS_HEADER) + STRING_NEWLINE;
	for(int connectionIndex=0; connectionIndex<ADCconnectionsData.size(); connectionIndex++)
	{
		vector<string>* ADCconnection = &(ADCconnectionsData[connectionIndex]);
		string preID = (*ADCconnection)[INDEXED_CSV_DATABASE_PREPROCESS_ADC_CONNECTIONS_IDS_DATAFILE_FIELD_INDEX_PRE_ID];
		string postID = (*ADCconnection)[INDEXED_CSV_DATABASE_PREPROCESS_ADC_CONNECTIONS_IDS_DATAFILE_FIELD_INDEX_POST_ID];
		string preClassLabel = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_UNKNOWN;
		string postClassLabel = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_CLASS_LABEL_UNKNOWN;
		string connectionSynNum = (*ADCconnection)[INDEXED_CSV_DATABASE_PREPROCESS_ADC_CONNECTIONS_IDS_DATAFILE_FIELD_INDEX_SYN_COUNT];
		string connectionNeurotransmitterType = (*ADCconnection)[INDEXED_CSV_DATABASE_PREPROCESS_ADC_CONNECTIONS_IDS_DATAFILE_FIELD_INDEX_NT_TYPE];
		string excitationTypeString = "";
		if(connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
		{
			if(neuronNeurotransmitterTypeMap.count(preID) != 0)
			{
				string neuronNeurotransmitterType = neuronNeurotransmitterTypeMap[preID];
				if(neuronExcitationTypeMap.count(neuronNeurotransmitterType) != 0)
				{
					excitationTypeString = SHAREDvars.convertIntToString(getNeuronExcitationTypeFromType(neuronNeurotransmitterType, &neuronExcitationTypeMap));
				}
				else
				{
					//cerr << "LCindexedCSVdatabasePreprocessClass::generateLocalConnectomeConnectionsDatasetFromADCconnectionsFile error: neuronExcitationTypeMap.count(neuronNeurotransmitterType) != 0; neuronNeurotransmitterType = " << neuronNeurotransmitterType << endl;
					excitationTypeString = SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_CONNECTIONS_EXCITATION_TYPE_UNKNOWN);
				}
			}
			else
			{
				cerr << "LCindexedCSVdatabasePreprocessClass::generateLocalConnectomeConnectionsDatasetFromADCconnectionsFile error: neuronNeurotransmitterTypeMap[preID] == 0" << endl;
				excitationTypeString = SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_CONNECTIONS_EXCITATION_TYPE_UNKNOWN);
			}
		}
		else
		{
			excitationTypeString = SHAREDvars.convertIntToString(getNeuronExcitationTypeFromType(connectionNeurotransmitterType, &neuronExcitationTypeMap));
		}

		//pre_id, post_id, pre_class_label, post_class_label, syn_num, excitation_type
		string neuronDatasetLine = preID + CSV_DELIMITER_CHAR + postID + CSV_DELIMITER_CHAR + preClassLabel + CSV_DELIMITER_CHAR + postClassLabel + CSV_DELIMITER_CHAR + connectionSynNum + CSV_DELIMITER_CHAR + excitationTypeString;
		neuronDatasetDataString += neuronDatasetLine + STRING_NEWLINE;
	}

	bool write = true;
	if(write)
	{
		bool appendToFile = false;
		ofstream writeFileObject = LCindexedCSVdatabaseOperations.prepareLocalConnectomeDatasetWrite(write, appendToFile, connectionDatasetFileNameWrite);
		SHAREDvars.writeStringToFileObject(neuronDatasetDataString, &writeFileObject);
		LCindexedCSVdatabaseOperations.finaliseLocalConnectomeDatasetWrite(write, &writeFileObject);
	}
}
#endif


#ifdef INDEXED_CSV_DATABASE_LDC
#ifdef INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_NEURONS_DATASET

void LCindexedCSVdatabasePreprocessClass::generateLocalConnectomeNeuronsDatasetFromSkeletons(const string local_connectome_folder_base, const string neuronDatasetFileNameWrite)
{	
	vector<string> neuronIDlist = getNeuronIDlist(local_connectome_folder_base, true);
	vector<string> neuronTypeList = getNeuronTypeList(local_connectome_folder_base, &neuronIDlist);
	vector<vecString> neuronPOSlist = getNeuronPosList(local_connectome_folder_base, &neuronIDlist);
	map<string, string> neuronExcitationTypeMap = calculateNeuronExcitationTypeMap(local_connectome_folder_base, INDEXED_CSV_DATABASE_PREPROCESS_NEURON_TYPES_FILE_NAME);
	
	string neuronDatasetDataString = string(LOCAL_CONNECTOME_DATASET_NEURONS_HEADER) + STRING_NEWLINE;
	for(int neuronIndex=0; neuronIndex<neuronIDlist.size(); neuronIndex++)
	{
		string excitationTypeString = SHAREDvars.convertIntToString(getNeuronExcitationTypeFromType(neuronTypeList[neuronIndex], &neuronExcitationTypeMap));
		string neuronDatasetLine = neuronIDlist[neuronIndex] + CSV_DELIMITER_CHAR + neuronPOSlist[neuronIndex].x + CSV_DELIMITER_CHAR + neuronPOSlist[neuronIndex].y + CSV_DELIMITER_CHAR + neuronPOSlist[neuronIndex].z + CSV_DELIMITER_CHAR + neuronTypeList[neuronIndex] + CSV_DELIMITER_CHAR + excitationTypeString; 
		neuronDatasetDataString += neuronDatasetLine + STRING_NEWLINE;
	}
	
	bool write = true;
	if(write)
	{
		bool appendToFile = false;
		ofstream writeFileObject = LCindexedCSVdatabaseOperations.prepareLocalConnectomeDatasetWrite(write, appendToFile, neuronDatasetFileNameWrite);
		SHAREDvars.writeStringToFileObject(neuronDatasetDataString, &writeFileObject);
		LCindexedCSVdatabaseOperations.finaliseLocalConnectomeDatasetWrite(write, &writeFileObject);
	}
}

vector<string> LCindexedCSVdatabasePreprocessClass::getNeuronIDlist(const string local_connectome_folder_base, const bool writeToFile)
{
	vector<string> neuronIDlist;

	LCindexedCSVdatabaseOperations.prepareLocalConnectomeDatasetRead(local_connectome_folder_base);
	int smSkeletonIDsDataFileSize = 0;
	vector<vector<string>> smSkeletonIDsData;
	bool expectFirstLineHeader = true;
	SHAREDvars.getLinesFromFileCSV(INDEXED_CSV_DATABASE_PREPROCESS_LDC_SM_SKELETON_IDS_FILE_NAME, &smSkeletonIDsData, &smSkeletonIDsDataFileSize, CSV_DELIMITER_CHAR, expectFirstLineHeader);
	
	string neuronIDlistDataString;
	for(int neuronIndex=0; neuronIndex<smSkeletonIDsDataFileSize; neuronIndex++)
	{
		vector<string>* row = &(smSkeletonIDsData[neuronIndex]);
		string neuronID = (*row)[0];
		neuronIDlist.push_back(neuronID);
		neuronIDlistDataString += neuronID + STRING_NEWLINE;
	}
	
	if(writeToFile)
	{
		bool appendToFile = false;
		ofstream writeFileObject = LCindexedCSVdatabaseOperations.prepareLocalConnectomeDatasetWrite(writeToFile, appendToFile, INDEXED_CSV_DATABASE_PREPROCESS_NEURON_LIST_FILE_NAME);
		SHAREDvars.writeStringToFileObject(neuronIDlistDataString, &writeFileObject);
		LCindexedCSVdatabaseOperations.finaliseLocalConnectomeDatasetWrite(writeToFile, &writeFileObject);
	}
	
	return neuronIDlist;
}

vector<string> LCindexedCSVdatabasePreprocessClass::getNeuronTypeList(const string local_connectome_folder_base, vector<string>* neuronIDlist)
{	
	vector<string> neuronTypeList;

	map<string, int> neuronMap;
	for(int neuronIndex=0; neuronIndex<neuronIDlist->size(); neuronIndex++)
	{
		string neuronID = (*neuronIDlist)[neuronIndex];
		//int neuronIDint = SHAREDvars.convertStringToInt(neuronID);
    	(neuronMap)[neuronID] = neuronIndex;
		neuronTypeList.push_back(LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_TYPE_UNKNOWN);
	}	

	LCindexedCSVdatabaseOperations.prepareLocalConnectomeDatasetRead(local_connectome_folder_base);
	
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS_LDC_SM_CELLTYPE_DATAFILE_2
	{
		int smCelltypeDataFileSize = 0;
		vector<vector<string>> smCelltypeData;
		bool expectFirstLineHeader = true;
		SHAREDvars.getLinesFromFileCSV(INDEXED_CSV_DATABASE_PREPROCESS_LDC_SM_CELLTYPE_DATAFILE_2_NAME, &smCelltypeData, &smCelltypeDataFileSize, CSV_DELIMITER_CHAR, expectFirstLineHeader);
		for(int rowIndex=0; rowIndex<smCelltypeDataFileSize; rowIndex++)
		{
			vector<string>* row = &(smCelltypeData[rowIndex]);
			string rowNeuronID1 = (*row)[INDEXED_CSV_DATABASE_PREPROCESS_LDC_SM_CELLTYPE_DATAFILE_2_FIELD_INDEX_SKID1];
			string rowNeuronID2 = (*row)[INDEXED_CSV_DATABASE_PREPROCESS_LDC_SM_CELLTYPE_DATAFILE_2_FIELD_INDEX_SKID2];
			string rowNeuronType = (*row)[INDEXED_CSV_DATABASE_PREPROCESS_LDC_SM_CELLTYPE_DATAFILE_2_FIELD_INDEX_CELLTYPE];
			/*
			cout << "rowNeuronID1 = " << rowNeuronID1 << endl;
			cout << "rowNeuronID2 = " << rowNeuronID2 << endl;
			cout << "rowNeuronType = " << rowNeuronType << endl;
			*/
			if(neuronMap.find(rowNeuronID1) != neuronMap.end())
			{
				int neuronListIndex = neuronMap[rowNeuronID1];
				neuronTypeList[neuronListIndex] = rowNeuronType;
			}
			if(neuronMap.find(rowNeuronID2) != neuronMap.end())
			{
				int neuronListIndex = neuronMap[rowNeuronID2];
				neuronTypeList[neuronListIndex] = rowNeuronType;
			}
		}
	}
	#endif
	#ifdef INDEXED_CSV_DATABASE_PREPROCESS_LDC_SM_CELLTYPE_DATAFILE_34
	{
		int smCelltypeDataFileSize = 0;
		vector<vector<string>> smCelltypeData;
		bool expectFirstLineHeader = true;
		SHAREDvars.getLinesFromFileCSV(INDEXED_CSV_DATABASE_PREPROCESS_LDC_SM_CELLTYPE_DATAFILE_3_NAME, &smCelltypeData, &smCelltypeDataFileSize, CSV_DELIMITER_CHAR, expectFirstLineHeader);
		SHAREDvars.getLinesFromFileCSV(INDEXED_CSV_DATABASE_PREPROCESS_LDC_SM_CELLTYPE_DATAFILE_4_NAME, &smCelltypeData, &smCelltypeDataFileSize, CSV_DELIMITER_CHAR, expectFirstLineHeader);
		for(int rowIndex=0; rowIndex<smCelltypeDataFileSize; rowIndex++)
		{
			vector<string>* row = &(smCelltypeData[rowIndex]);
			string rowNeuronID = (*row)[INDEXED_CSV_DATABASE_PREPROCESS_LDC_SM_CELLTYPE_DATAFILE_34_FIELD_INDEX_SKID];
			string rowNeuronType = (*row)[INDEXED_CSV_DATABASE_PREPROCESS_LDC_SM_CELLTYPE_DATAFILE_34_FIELD_INDEX_CELLTYPE];
			/*
			cout << "rowNeuronID = " << rowNeuronID << endl;
			cout << "rowNeuronType = " << rowNeuronType << endl;
			*/
			if(neuronMap.find(rowNeuronID) != neuronMap.end())
			{
				int neuronListIndex = neuronMap[rowNeuronID];
				neuronTypeList[neuronListIndex] = rowNeuronType;
			}
			else
			{
				cerr << "LCindexedCSVdatabasePreprocessClass::getNeuronTypeList error: key not found:  rowNeuronID = " << rowNeuronID << endl;
			}
		}
	}
	#endif
	
	return neuronTypeList;
}
	
vector<vecString> LCindexedCSVdatabasePreprocessClass::getNeuronPosList(const string local_connectome_folder_base, vector<string>* neuronIDlist)
{	
	vector<vecString> neuronPOSlist;

	map<string, int> neuronMap;
	for(int neuronIndex=0; neuronIndex<neuronIDlist->size(); neuronIndex++)
	{
		string neuronID = (*neuronIDlist)[neuronIndex];
		string skeletonFileName = string(INDEXED_CSV_DATABASE_PREPROCESS_LDC_SKELETON_DATAFILE_NAME_START) + neuronID + INDEXED_CSV_DATABASE_PREPROCESS_LDC_SKELETON_DATAFILE_NAME_EXTENSION;
		string skeletonsFolderName = string(LOCAL_CONNECTOME_DATASET_FOLDER) + INDEXED_CSV_DATABASE_PREPROCESS_LDC_SKELETON_DATAFILE_FOLDER_NAME;
		/*
		cout << "skeletonFileName = " << skeletonFileName << endl;
		cout << "skeletonsFolderName = " << skeletonsFolderName << endl;
		*/
		LCindexedCSVdatabaseOperations.prepareLocalConnectomeDatasetReadCustom(local_connectome_folder_base, skeletonsFolderName);
		int skeletonDataFileSize = 0;
		vector<vector<string>> skeletonData;
		bool expectFirstLineHeader = false;
		SHAREDvars.getLinesFromFileCSV(skeletonFileName, &skeletonData, &skeletonDataFileSize, CHAR_SPACE, expectFirstLineHeader);
		
		vec neuronPosCentroid = {0.0, 0.0, 0.0};
		for(int nodeIndex=0; nodeIndex<skeletonData.size(); nodeIndex++)
		{
			vec pos; 
			vector<string>* row = &(skeletonData[nodeIndex]);						
			pos.x = SHAREDvars.convertStringToDouble((*row)[INDEXED_CSV_DATABASE_PREPROCESS_LDC_SKELETON_DATAFILE_NODE_FIELD_INDEX_X]);
			pos.y = SHAREDvars.convertStringToDouble((*row)[INDEXED_CSV_DATABASE_PREPROCESS_LDC_SKELETON_DATAFILE_NODE_FIELD_INDEX_Y]);
			pos.z = SHAREDvars.convertStringToDouble((*row)[INDEXED_CSV_DATABASE_PREPROCESS_LDC_SKELETON_DATAFILE_NODE_FIELD_INDEX_Z]);
			/*
			cout << "pos.x = " << pos.x << endl;
			cout << "pos.y = " << pos.y << endl;
			cout << "pos.z = " << pos.z << endl;
			*/
			neuronPosCentroid.x += pos.x;
			neuronPosCentroid.y += pos.y;
			neuronPosCentroid.z += pos.z;
		}
		neuronPosCentroid.x /= skeletonData.size();
		neuronPosCentroid.y /= skeletonData.size();
		neuronPosCentroid.z /= skeletonData.size();
		
		vecString neuronPosCentroidString;
		string doubleFormat = "%0.3f";
		neuronPosCentroidString.x = SHAREDvars.convertDoubleToString(neuronPosCentroid.x, doubleFormat);
		neuronPosCentroidString.y = SHAREDvars.convertDoubleToString(neuronPosCentroid.y, doubleFormat);
		neuronPosCentroidString.z = SHAREDvars.convertDoubleToString(neuronPosCentroid.z, doubleFormat);
		
		neuronPOSlist.push_back(neuronPosCentroidString);
	}	

	return neuronPOSlist;
}
	
#endif

#ifdef INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_LOCAL_CONNECTOME_CONNECTIONS_DATASET

void LCindexedCSVdatabasePreprocessClass::generateLocalConnectomeConnectionsDatasetFromMatrix(const string local_connectome_folder_base, const string connectionDatasetFileNameWrite)
{
	ofstream writeFileObject = LCindexedCSVdatabaseOperations.prepareLocalConnectomeDatasetWrite(true, false, connectionDatasetFileNameWrite);

	LCindexedCSVdatabaseOperations.prepareLocalConnectomeDatasetRead(local_connectome_folder_base);
	
	string neuronConnectionsFileHeader = string(LOCAL_CONNECTOME_DATASET_CONNECTIONS_HEADER) + STRING_NEWLINE;
	SHAREDvars.writeStringToFileObject(neuronConnectionsFileHeader, &writeFileObject);

	#ifdef INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_CONNECTIONS_EXCITATION_TYPE
	map<string, string> neuronRegionTypeMap = calculateNeuronRegionTypeMap(local_connectome_folder_base);
	map<string, string> neuronExcitationTypeMap = calculateNeuronExcitationTypeMap(local_connectome_folder_base, INDEXED_CSV_DATABASE_PREPROCESS_NEURON_TYPES_FILE_NAME);
	#else
	map<string, string> neuronRegionTypeMap, neuronExcitationTypeMap;
	#endif
	
	for(int matrixFileIndex=0; matrixFileIndex<LOCAL_CONNECTOME_CONNECTIONS_MATRIX_NUMBER_OF_FILES; matrixFileIndex++)
	{
		cout << "matrixFileIndex = " << matrixFileIndex << endl;
		string matrixFileName = localConnectomeConnectionsMatrixFileNames[matrixFileIndex];
		string matrixFilePreClassLabel = localConnectomeConnectionsMatrixSynapsePreClassLabel[matrixFileIndex];
		string matrixFilePostClassLabel = localConnectomeConnectionsMatrixSynapsePostClassLabel[matrixFileIndex];
		convertConnectionsListMatrixFile(local_connectome_folder_base, matrixFileName, matrixFilePreClassLabel, matrixFilePostClassLabel, &writeFileObject, &neuronRegionTypeMap, &neuronExcitationTypeMap);
	} 
	
	LCindexedCSVdatabaseOperations.finaliseLocalConnectomeDatasetWrite(true, &writeFileObject);
}

void LCindexedCSVdatabasePreprocessClass::convertConnectionsListMatrixFile(const string local_connectome_folder_base, const string matrixFileName, const string matrixFilePreClassLabel, const string matrixFilePostClassLabel, ofstream* writeFileObject, map<string, string>* neuronRegionTypeMap, map<string, string>* neuronExcitationTypeMap)
{	
	//matrix format: Row neurons are presynaptic, while column neurons are postsynaptic, first row/column contain neuronID (skid)
		
	int matrixFileSize = 0;
	vector<vector<string>> matrixFileData;
	bool expectFirstLineHeader = false;
	SHAREDvars.getLinesFromFileCSV(matrixFileName, &matrixFileData, &matrixFileSize, CSV_DELIMITER_CHAR, expectFirstLineHeader);
	int numberOfNeurons = matrixFileSize-1;	//ignore header
	
	cout << "matrixFileName = " << matrixFileName << endl;
	cout << "numberOfNeurons = " << numberOfNeurons << endl;
	
	vector<string> neuronIDlist = matrixFileData[0];	//skid
	neuronIDlist.erase(neuronIDlist.begin());
	/*
	vector<string> matrixFileHeader = matrixFileData[0];
	vector<string> neuronIDlist;
	for(int neuronIndex=1; neuronIndex<matrixFileSize; neuronIndex++)
	{
		string neuronID = matrixFileHeader[neuronIndex];	//skid
		neuronIDlist.push_back(neuronID);
	}
	*/
		
	for(int sourceNeuronIndex=0; sourceNeuronIndex<numberOfNeurons; sourceNeuronIndex++)
	{
		vector<string> matrixFileDataRow = matrixFileData[sourceNeuronIndex+1];
		for(int targetNeuronIndex=0; targetNeuronIndex<numberOfNeurons; targetNeuronIndex++)
		{
			string matrixFileDataCell = matrixFileDataRow[targetNeuronIndex+1];
			int connectionNumberOfSynapsesInt = int(SHAREDvars.convertStringToFloat(matrixFileDataCell));
			if(connectionNumberOfSynapsesInt > 0)
			{
				string connectionNumberOfSynapses = SHAREDvars.convertIntToString(connectionNumberOfSynapsesInt);

				string connectionsSourceNeuronID = neuronIDlist[sourceNeuronIndex];
				string connectionsTargetNeuronID = neuronIDlist[targetNeuronIndex];
				
				#ifdef INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_CONNECTIONS_EXCITATION_TYPE
				string excitationTypeString = "";
				if(neuronRegionTypeMap->count(connectionsSourceNeuronID) != 0)
				{
					string neuronRegionType = (*neuronRegionTypeMap)[connectionsSourceNeuronID];
					excitationTypeString = SHAREDvars.convertIntToString(getNeuronExcitationTypeFromType(neuronRegionType, neuronExcitationTypeMap));
				}
				else
				{
					//cerr << "LCindexedCSVdatabasePreprocessClass::convertConnectionsListMatrixFile error: neuronRegionTypeMap[preID] == 0" << endl;
					excitationTypeString = SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_CONNECTIONS_EXCITATION_TYPE_UNKNOWN);
				}
				#endif 
				
				string neuronConnectionString = 
				connectionsSourceNeuronID + CSV_DELIMITER + 
				connectionsTargetNeuronID + CSV_DELIMITER +  
				matrixFilePreClassLabel + CSV_DELIMITER + matrixFilePostClassLabel + CSV_DELIMITER + connectionNumberOfSynapses + 
				#ifdef INDEXED_CSV_DATABASE_PREPROCESS_GENERATE_CONNECTIONS_EXCITATION_TYPE
				CSV_DELIMITER + excitationTypeString +
				#endif
				STRING_NEWLINE;
				/*
				string neuronConnectionString = 
				connectionsSourceNeuronID + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsSourceNeuronCoordinates.x) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsSourceNeuronCoordinates.y) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsSourceNeuronCoordinates.z) + CSV_DELIMITER + connectionsSourceNeuronType + CSV_DELIMITER + 
				connectionsTargetNeuronID + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsTargetNeuronCoordinates.x) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsTargetNeuronCoordinates.y) + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionsTargetNeuronCoordinates.z) + CSV_DELIMITER + connectionsTargetNeuronType + CSV_DELIMITER + 
				matrixFileClassLabel + CSV_DELIMITER + connectionNumberOfSynapses + CSV_DELIMITER + SHAREDvars.convertIntToString(connectionExcitationType) + STRING_NEWLINE;
				*/

				//cout << "neuronConnectionString = " << neuronConnectionString << endl;

				SHAREDvars.writeStringToFileObject(neuronConnectionString, writeFileObject);
			}
		}
	}
}

#endif

#endif

int LCindexedCSVdatabasePreprocessClass::getNeuronExcitationTypeFromType(const string neuronType, map<string, string>* neuronExcitationTypeMap)
{
	string excitationTypeString = (*neuronExcitationTypeMap)[neuronType];
	if(neuronExcitationTypeMap->count(neuronType) == 0)
	{
		cerr << "getNeuronExcitationTypeFromType error: neuronExcitationTypeMap[neuronType] not found; neuronType = " << neuronType << endl;
	}
	//cout << "excitationTypeString = " << excitationTypeString << endl;
	int excitationType = SHAREDvars.convertStringToInt(excitationTypeString);
	//cout << "excitationType = " << excitationType << endl;
	return excitationType;
}

map<string, string> LCindexedCSVdatabasePreprocessClass::calculateNeuronExcitationTypeMap(const string local_connectome_folder_base, const string neuronTypesFileName)
{
	return calculateNeuronTypeMap(local_connectome_folder_base, neuronTypesFileName, INDEXED_CSV_DATABASE_PREPROCESS_NEURON_TYPES_FIELD_INDEX_NEURONTYPE, INDEXED_CSV_DATABASE_PREPROCESS_NEURON_TYPES_FIELD_INDEX_NEURONVALUE, CHAR_TAB);
}
#ifdef INDEXED_CSV_DATABASE_LDC
map<string, string> LCindexedCSVdatabasePreprocessClass::calculateNeuronRegionTypeMap(const string local_connectome_folder_base)
{
	map<string, string> neuronTypeMap;
	calculateNeuronTypeMap(local_connectome_folder_base, INDEXED_CSV_DATABASE_PREPROCESS_LDC_SM_CELLTYPE_DATAFILE_3_NAME, INDEXED_CSV_DATABASE_PREPROCESS_LDC_SM_CELLTYPE_DATAFILE_34_FIELD_INDEX_SKID, INDEXED_CSV_DATABASE_PREPROCESS_LDC_SM_CELLTYPE_DATAFILE_34_FIELD_INDEX_CELLTYPE, CSV_DELIMITER_CHAR, &neuronTypeMap, false);
	calculateNeuronTypeMap(local_connectome_folder_base, INDEXED_CSV_DATABASE_PREPROCESS_LDC_SM_CELLTYPE_DATAFILE_4_NAME, INDEXED_CSV_DATABASE_PREPROCESS_LDC_SM_CELLTYPE_DATAFILE_34_FIELD_INDEX_SKID, INDEXED_CSV_DATABASE_PREPROCESS_LDC_SM_CELLTYPE_DATAFILE_34_FIELD_INDEX_CELLTYPE, CSV_DELIMITER_CHAR, &neuronTypeMap, false);
	return neuronTypeMap;
}
#endif
#ifdef INDEXED_CSV_DATABASE_ADC
map<string, string> LCindexedCSVdatabasePreprocessClass::calculateNeuronCoordinatesTypeMap(const string local_connectome_folder_base)
{
	return calculateNeuronTypeMap(local_connectome_folder_base, INDEXED_CSV_DATABASE_PREPROCESS_ADC_COORDINATES_DATAFILE_NAME, INDEXED_CSV_DATABASE_PREPROCESS_ADC_COORDINATES_DATAFILE_FIELD_INDEX_ID, INDEXED_CSV_DATABASE_PREPROCESS_ADC_COORDINATES_DATAFILE_FIELD_INDEX_POSITION, CSV_DELIMITER_CHAR);
}
map<string, string> LCindexedCSVdatabasePreprocessClass::calculateNeuronNeurotransmitterTypeMap(const string local_connectome_folder_base)
{
	return calculateNeuronTypeMap(local_connectome_folder_base, INDEXED_CSV_DATABASE_PREPROCESS_ADC_NEURONS_DATAFILE_NAME, INDEXED_CSV_DATABASE_PREPROCESS_ADC_NEURONS_DATAFILE_FIELD_INDEX_ID, INDEXED_CSV_DATABASE_PREPROCESS_ADC_NEURONS_DATAFILE_FIELD_INDEX_NT_TYPE, CSV_DELIMITER_CHAR);
}
#endif
#ifdef INDEXED_CSV_DATABASE_CEC
map<string, string> LCindexedCSVdatabasePreprocessClass::calculateNeuronIDMap(const string local_connectome_folder_base)
{
	map<string, string> neuronTypeMap;
	calculateNeuronTypeMap(local_connectome_folder_base, INDEXED_CSV_DATABASE_PREPROCESS_CEC_DISTANCES_DATAFILE_NAME, INDEXED_CSV_DATABASE_PREPROCESS_CEC_DISTANCES_DATAFILE_FIELD_INDEX_NEURON, INT_IRRELEVANT, CSV_DELIMITER_CHAR, &neuronTypeMap, true);
	return neuronTypeMap;
}
#endif
map<string, string> LCindexedCSVdatabasePreprocessClass::calculateNeuronTypeMap(const string local_connectome_folder_base, const string neuronTypesFileName, const int neuronTypeIndex, const int neuronValueIndex, const char delimiter)
{
	map<string, string> neuronTypeMap;
	calculateNeuronTypeMap(local_connectome_folder_base, neuronTypesFileName, neuronTypeIndex, neuronValueIndex, delimiter, &neuronTypeMap, false);
	return neuronTypeMap;
}

void LCindexedCSVdatabasePreprocessClass::calculateNeuronTypeMap(const string local_connectome_folder_base, const string neuronTypesFileName, const int neuronTypeIndex, const int neuronValueIndex, const char delimiter, map<string, string>* neuronTypeMap, const bool deriveNeuronID)
{
	LCindexedCSVdatabaseOperations.prepareLocalConnectomeDatasetRead(local_connectome_folder_base);
	int neuronTypeFileSize = 0;
	vector<vector<string>> neuronTypeData;
	bool expectFirstLineHeader = true;
	SHAREDvars.getLinesFromFileCSV(neuronTypesFileName, &neuronTypeData, &neuronTypeFileSize, delimiter, expectFirstLineHeader);
		
	string neuronIDlistDataString;
	for(int rowIndex=0; rowIndex<neuronTypeFileSize; rowIndex++)
	{
		vector<string>* row = &(neuronTypeData[rowIndex]);
		string neuronType = "";
		if(deriveNeuronID)
		{
			string neuronID = SHAREDvars.convertIntToString(rowIndex);
			string neuronValue = (*row)[neuronValueIndex];
			(*neuronTypeMap)[neuronValue] = neuronID;
		}
		else
		{
			neuronType = (*row)[neuronTypeIndex];
			string neuronValue = (*row)[neuronValueIndex];
			//cout << "neuronType = " << neuronType << endl;
			//cout << "neuronValue = " << neuronValue << endl;
			(*neuronTypeMap)[neuronType] = neuronValue;
		}
	}
}

#endif
