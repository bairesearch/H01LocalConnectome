/*******************************************************************************
 *
 * File Name: LCindexedCSVdatabaseTraceLocalConnectome.cpp
 * Author: Richard Bruce Baxter - Copyright (c) 2021-2023 Baxter AI (baxterai.com)
 * License: MIT License
 * Project: LCLocalConnectome
 * Requirements: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Compilation: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Usage: see LCindexedCSVdatabaseGlobalDefs.hpp
 * Description: LC indexed CSV database trace local connectome - traces local connectome dataset (saving visualisation)
 * Input: localConnectomeNeurons.csv / localConnectomeConnectionsX.csv
 * Output Format: SVG (2D) / LDR (3D) [multiple files for video construction]
 * Comments:
 * /
 *******************************************************************************/

#include "LCindexedCSVdatabaseTraceLocalConnectome.hpp"

#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME

int traceIterationIndexSubMax = 0;

bool LCindexedCSVdatabaseTraceLocalConnectomeClass::traceLocalConnectomeCSVdataset(const string local_connectome_folder_base)
{
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_PRESYNAPTIC_NEURONS
	this->traceLocalConnectomeCSVdatasetFile(local_connectome_folder_base, READ_FILE_TRUE, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS, CONNECTION_TYPES_DERIVED_FROM_PRESYNAPTIC_NEURONS);
	#endif
	#ifdef INDEXED_CSV_DATABASE_QUERY_COUNT_CONNECTIONS_EXCITATION_TYPE_FROM_EM_IMAGES
	this->traceLocalConnectomeCSVdatasetFile(local_connectome_folder_base, READ_FILE_TRUE, LOCAL_CONNECTOME_DATASET_NEURONS_FILENAME, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, QUERY_POSTSYNAPTIC_CONNECTION_NEURONS, WRITE_FILE_TRUE, APPEND_FILE_FALSE, LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES, CONNECTION_TYPES_DERIVED_FROM_EM_IMAGES);
	#endif
}



bool LCindexedCSVdatabaseTraceLocalConnectomeClass::traceLocalConnectomeCSVdatasetFile(const string local_connectome_folder_base, const bool datasetRead, const string neuronDatasetFileNameRead, const string connectionDatasetFileNameRead, const bool queryPresynapticConnectionNeurons, const bool connectionDatasetWrite, const bool appendToFile, const string connectionDatasetFileNameWrite, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages)
{
	bool result = true;
		
	vector<vector<string>> localConnectomeCSVdatasetNeurons;
	vector<vector<string>> localConnectomeCSVdatasetConnections;
	vector<string> neuronList;
	#ifdef INDEXED_CSV_DATABASE_QUERY_EFFICIENT_STORE_NEURON_IDS_IN_MAP
	map<string, int> neuronMap;
	#endif
	map<string, int> connectionsMap;
			

	cout << "neuronDatasetFileNameRead = " << neuronDatasetFileNameRead << endl;
	cout << "connectionDatasetFileNameRead = " << connectionDatasetFileNameRead << endl;

	bool readConnections = true;
	bool buildConnectionsMap = false;
	bool readLayers = true; 
	bool readLayersConnections = true;

	if(!LCindexedCSVdatabaseCalculateNeuronLayer.readLocalNeuronsAndConnections(local_connectome_folder_base, neuronDatasetFileNameRead, true, &neuronList, &neuronMap, &localConnectomeCSVdatasetNeurons, readConnections, connectionDatasetFileNameRead, &localConnectomeCSVdatasetConnections, buildConnectionsMap, &connectionsMap, readLayers, readLayersConnections))
	{
		result = false;
	}
		
	//initialise connection/neuron trace highlight values;
	for(int i=0; i<localConnectomeCSVdatasetNeurons.size(); i++)
	{
		vector<string>* localConnectomeNeuron = &(localConnectomeCSVdatasetNeurons[i]);
		localConnectomeNeuron->push_back(SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_VALUE_INACTIVE));	//add field LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE
		localConnectomeNeuron->push_back(SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_HIGHLIGHT1_VALUE_INACTIVE));	//add field LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT1
		localConnectomeNeuron->push_back(SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_HIGHLIGHT2_VALUE_INACTIVE));	//add field LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT2
		#ifdef INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION
		localConnectomeNeuron->push_back(SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_ACTIVATION_LEVEL_INITIAL));	//add field LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_ACTIVATION_LEVEL_POSITIVE
		localConnectomeNeuron->push_back(SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_ACTIVATION_LEVEL_INITIAL));	//add field LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_ACTIVATION_LEVEL_NEGATIVE
		#endif
	}
	for(int i=0; i<localConnectomeCSVdatasetConnections.size(); i++)
	{
		vector<string>* localConnectomeConnection = &(localConnectomeCSVdatasetConnections[i]);
		localConnectomeConnection->push_back(SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_CONNECTION_TRACE_VALUE_INACTIVE));	//add field LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_TRACE
		localConnectomeConnection->push_back(SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_CONNECTION_TRACE_HIGHLIGHT1_VALUE_INACTIVE));	//add field LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT1
		localConnectomeConnection->push_back(SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_CONNECTION_TRACE_HIGHLIGHT2_VALUE_INACTIVE));	//add field LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT2
	}
	
	int traceIterationIndex = 0;
	this->traceLocalConnectomeCSVdataset(local_connectome_folder_base, &localConnectomeCSVdatasetNeurons, &localConnectomeCSVdatasetConnections, &neuronMap, &connectionsMap, queryPresynapticConnectionNeurons, connectionDatasetWrite, appendToFile, traceIterationIndex, connectionDatasetFileNameWrite, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, LAYER_ENFORCEMENT_TRUE, INDEXED_CSV_DATABASE_TRACE_START_LAYER_INDEX);

	return result;
}

bool LCindexedCSVdatabaseTraceLocalConnectomeClass::traceLocalConnectomeCSVdataset(const string local_connectome_folder_base, vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, map<string, int>* neuronMap, map<string, int>* connectionsMap, const bool queryPresynapticConnectionNeurons, const bool connectionDatasetWrite, const bool appendToFile, const int traceIterationIndex, const string connectionDatasetFileNameWrite, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool layerIndexEnforcement, const int layerIndex)
{
	bool result = true;
		
	//cout << "\nLCindexedCSVdatabaseTraceLocalConnectomeClass::traceLocalConnectomeCSVdataset: traceIterationIndex = " << traceIterationIndex << endl;
	
	//perform trace: update neuron trace;
	for(int i=0; i<localConnectomeCSVdatasetNeurons->size(); i++)
	{
		vector<string>* localConnectomeNeuron = &((*localConnectomeCSVdatasetNeurons)[i]);
		
		int localConnectomeNeuronLayer = SHAREDvars.convertStringToInt((*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_LAYER]);
		int neuronTraceValue = SHAREDvars.convertStringToInt((*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE]);
		//cout << "neuronTraceValue = " << neuronTraceValue << endl;
	
		//if draw but not trace inhibitory connections need to make sure then need to deactivate previous inhibitory neurons
		string neuronType = (*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];
		int neuronExcitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;
		if(neuronType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_PYRAMIDAL)
		{
			neuronExcitationType = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY;
		}
		else if(neuronType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_INTERNEURON)
		{
			neuronExcitationType = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY;
		}

		//if neuron on cortex entry layer;
		if(layerIndexEnforcement && (localConnectomeNeuronLayer == layerIndex))
		{
			#ifdef INDEXED_CSV_DATABASE_TRACE_START_LAYER_EXCITATORY_NEURONS
			if(neuronExcitationType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
			{
			#endif
				#ifdef INDEXED_CSV_DATABASE_TRACE_START_LAYER_SUBSET_RANDOM
				double randomNumberBetween0And1 = ((double) rand() / (RAND_MAX));
				if(randomNumberBetween0And1 < INDEXED_CSV_DATABASE_TRACE_START_LAYER_SUBSET_RANDOM_FRACTION)
				{
				#endif
					neuronTraceValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_VALUE_SOURCE;
				#ifdef INDEXED_CSV_DATABASE_TRACE_START_LAYER_SUBSET_RANDOM
				}
				#endif
			#ifdef INDEXED_CSV_DATABASE_TRACE_START_LAYER_EXCITATORY_NEURONS
			}
			#endif
		}
		else
		{
			//or if neuron has previously been traced;
			if(neuronTraceValue == INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_VALUE_SOURCE)
			{
				neuronTraceValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_VALUE_INACTIVE;
			}
			else if(neuronTraceValue == INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_VALUE_TARGET)
			{
				#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_TRACE_INHIBITORY_CONNECTIONS
				neuronTraceValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_VALUE_SOURCE;
				#else
				//if draw but not trace inhibitory connections need to make sure then need to deactivate previous inhibitory neurons;
				if(neuronExcitationType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
				{
					neuronTraceValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_VALUE_SOURCE;
				}
				else
				{
					neuronTraceValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_VALUE_INACTIVE;
				}
				#endif
			}
			else if(neuronTraceValue == INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_VALUE_INACTIVE)
			{
			}
		}
		
		(*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE] = SHAREDvars.convertIntToString(neuronTraceValue);
	}

	//clear neuron traceHighlight, traceMask;
	for(int i=0; i<localConnectomeCSVdatasetNeurons->size(); i++)
	{
		vector<string>* localConnectomeNeuron = &((*localConnectomeCSVdatasetNeurons)[i]);
		(*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT1] = SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_HIGHLIGHT1_VALUE_INACTIVE);
		(*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT2] = SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_HIGHLIGHT2_VALUE_INACTIVE);
		#ifdef INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION
		(*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_ACTIVATION_LEVEL_POSITIVE] = SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_ACTIVATION_LEVEL_INITIAL);
		(*localConnectomeNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_ACTIVATION_LEVEL_NEGATIVE] = SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_ACTIVATION_LEVEL_INITIAL);
		#endif
	}
	
	//clear connection trace, traceHighlight, traceMask;
	for(int i=0; i<localConnectomeCSVdatasetConnections->size(); i++)
	{
		vector<string>* localConnectomeConnection = &((*localConnectomeCSVdatasetConnections)[i]);
		(*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_TRACE] = SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_CONNECTION_TRACE_VALUE_INACTIVE);
		(*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT1] = SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_CONNECTION_TRACE_HIGHLIGHT1_VALUE_INACTIVE);
		(*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT2] = SHAREDvars.convertIntToString(INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_CONNECTION_TRACE_HIGHLIGHT2_VALUE_INACTIVE);
		
	}

	#ifdef INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION
	//adjust target neuron input activation level;
	for(int i=0; i<localConnectomeCSVdatasetConnections->size(); i++)
	{
		vector<string>* localConnectomeConnection = &((*localConnectomeCSVdatasetConnections)[i]);
		bool activateConnection = false;
		string connectionSourceNeuronID = (*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ID];
		string connectionTargetNeuronID = (*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ID];
		#ifdef LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES_STILL_REQUIRING_COMPLETION
		if((connectionSourceNeuronID != LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES_UNKNOWN_NEURON_ID) && (connectionTargetNeuronID != LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES_UNKNOWN_NEURON_ID))
		{
		#endif
			int sourceNeuronIndex = (*neuronMap)[connectionSourceNeuronID];
			vector<string>* connectionSourceNeuron = &((*localConnectomeCSVdatasetNeurons)[sourceNeuronIndex]);
			int sourceNeuronTraceValue = SHAREDvars.convertStringToInt((*connectionSourceNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE]);
			if(sourceNeuronTraceValue == INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_VALUE_SOURCE)
			{
				activateConnection = true;
			}
	
			int targetNeuronIndex = (*neuronMap)[connectionTargetNeuronID];
			vector<string>* connectionTargetNeuron = &((*localConnectomeCSVdatasetNeurons)[targetNeuronIndex]);
			if(activateConnection)
			{
				int activationLevelPositiveCurrent = SHAREDvars.convertStringToInt((*connectionTargetNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_ACTIVATION_LEVEL_POSITIVE]);
				int activationLevelNegativeCurrent = SHAREDvars.convertStringToInt((*connectionTargetNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_ACTIVATION_LEVEL_NEGATIVE]);
				int connectionExcitationType = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE]);		
				if(connectionExcitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
				{
					activationLevelPositiveCurrent = activationLevelPositiveCurrent + INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_ACTIVATION_LEVEL_RESOLUTION;
				}
				else if(connectionExcitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
				{
					activationLevelNegativeCurrent = activationLevelNegativeCurrent + INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_ACTIVATION_LEVEL_RESOLUTION;
				}
				(*connectionTargetNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_ACTIVATION_LEVEL_POSITIVE] = SHAREDvars.convertIntToString(activationLevelPositiveCurrent);
				(*connectionTargetNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_ACTIVATION_LEVEL_NEGATIVE] = SHAREDvars.convertIntToString(activationLevelNegativeCurrent);
			}
		#ifdef LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES_STILL_REQUIRING_COMPLETION
		}
		#endif
	}
	#endif
				
	//perform trace: highlight neurons/connections;
	int traceIterationIndexSub = 0;
	#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_SUB
	
	#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_SUB_CONNECTION_SOURCES
	this->traceLocalConnectomeCSVdatasetHighlight(
	false, false, false, false, false, false, 
	false, false, false, false, false, false, 
	false, false, false, false, false, false, false, false,
	local_connectome_folder_base, localConnectomeCSVdatasetNeurons, localConnectomeCSVdatasetConnections, neuronMap, connectionsMap, queryPresynapticConnectionNeurons, connectionDatasetWrite, appendToFile, traceIterationIndex, traceIterationIndexSub, connectionDatasetFileNameWrite, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, layerIndexEnforcement, layerIndex);
	traceIterationIndexSub = traceIterationIndexSub + 1;	
	#endif
	#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_SUB_CONNECTION
	this->traceLocalConnectomeCSVdatasetHighlight(
	true, true, false, false, false, false, 
	false, false, false, false, false, false, 
	false, false, false, false, false, false, false, false,
	local_connectome_folder_base, localConnectomeCSVdatasetNeurons, localConnectomeCSVdatasetConnections, neuronMap, connectionsMap, queryPresynapticConnectionNeurons, connectionDatasetWrite, appendToFile, traceIterationIndex, traceIterationIndexSub, connectionDatasetFileNameWrite, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, layerIndexEnforcement, layerIndex);
	traceIterationIndexSub = traceIterationIndexSub + 1;
	#endif
	/*
	#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_SUB_CONNECTION_TARGETS_OF_INHIBITORY_CONNECTIONS
	this->traceLocalConnectomeCSVdatasetHighlight(
	true, true, false, false, false, false, 
	false, false, false, false, false, false, 
	false, true, false, false, false, true, false, false,
	local_connectome_folder_base, localConnectomeCSVdatasetNeurons, localConnectomeCSVdatasetConnections, neuronMap, connectionsMap, queryPresynapticConnectionNeurons, connectionDatasetWrite, appendToFile, traceIterationIndex, traceIterationIndexSub, connectionDatasetFileNameWrite, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, layerIndexEnforcement, layerIndex);
	traceIterationIndexSub = traceIterationIndexSub + 1;
	#endif
	*/
	#ifdef INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_HIGHLIGHT2_INHIBITED_NEURONS
	this->traceLocalConnectomeCSVdatasetHighlight(
	true, true, false, false, false, false, 
	false, false, false, false, false, false, 
	false, true, false, false, false, false, true, false,
	local_connectome_folder_base, localConnectomeCSVdatasetNeurons, localConnectomeCSVdatasetConnections, neuronMap, connectionsMap, queryPresynapticConnectionNeurons, connectionDatasetWrite, appendToFile, traceIterationIndex, traceIterationIndexSub, connectionDatasetFileNameWrite, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, layerIndexEnforcement, layerIndex);
	traceIterationIndexSub = traceIterationIndexSub + 1;	
	#endif
	#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_SUB_CONNECTION_TARGETS
	this->traceLocalConnectomeCSVdatasetHighlight(
	true, true, false, false, false, false, 
	true, true, false, false, false, false, 
	false, false, false, false, false, false, false, false,
	local_connectome_folder_base, localConnectomeCSVdatasetNeurons, localConnectomeCSVdatasetConnections, neuronMap, connectionsMap, queryPresynapticConnectionNeurons, connectionDatasetWrite, appendToFile, traceIterationIndex, traceIterationIndexSub, connectionDatasetFileNameWrite, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, layerIndexEnforcement, layerIndex);
	traceIterationIndexSub = traceIterationIndexSub + 1;
	#endif
	
	#else
	this->traceLocalConnectomeCSVdatasetHighlight(
	true, true, false, false, false, false, 
	true, true, false, false, false, false, 
	false, false, false, false, false, false, false, false,
	local_connectome_folder_base, localConnectomeCSVdatasetNeurons, localConnectomeCSVdatasetConnections, neuronMap, connectionsMap, queryPresynapticConnectionNeurons, connectionDatasetWrite, appendToFile, traceIterationIndex, traceIterationIndexSub, connectionDatasetFileNameWrite, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, layerIndexEnforcement, layerIndex);
	traceIterationIndexSub = traceIterationIndexSub + 1;	
	#endif
	
	traceIterationIndexSubMax = traceIterationIndexSub;
	
	if(traceIterationIndex < INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_MAX_NUMBER_ITERATIONS-1)
	{
		bool layerIndexEnforcementNext = LAYER_ENFORCEMENT_FALSE;	//checkthis
		this->traceLocalConnectomeCSVdataset(local_connectome_folder_base, localConnectomeCSVdatasetNeurons, localConnectomeCSVdatasetConnections, neuronMap, connectionsMap, queryPresynapticConnectionNeurons, connectionDatasetWrite, appendToFile, (traceIterationIndex+1), connectionDatasetFileNameWrite, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, layerIndexEnforcementNext, layerIndex);
	}
	
	return result;
}

bool LCindexedCSVdatabaseTraceLocalConnectomeClass::traceLocalConnectomeCSVdatasetHighlight(
	const bool drawConnectionExcitatory, const bool drawConnectionInhibitory, const bool highlightConnectionExcitatory, const bool highlightConnectionInhibitory, const bool markConnectionExcitatory, const bool markConnectionInhibitory, 
	const bool drawTargetExcitatory, const bool drawTargetInhibitory, const bool highlightTargetExcitatory, const bool highlightTargetInhibitory, const bool markTargetExcitatory, const bool markTargetInhibitory, 
	const bool drawTargetOfExcitatoryConnection, const bool drawTargetOfInhibitoryConnection, const bool highlightTargetOfExcitatoryConnection, const bool highlightTargetOfInhibitoryConnection, const bool markTargetOfExcitatoryConnection, const bool markTargetOfInhibitoryConnection, const bool markInhibitedNeurons, const bool markExcitedNeurons, 
	const string local_connectome_folder_base, vector<vector<string>>* localConnectomeCSVdatasetNeurons, vector<vector<string>>* localConnectomeCSVdatasetConnections, map<string, int>* neuronMap, map<string, int>* connectionsMap, const bool queryPresynapticConnectionNeurons, const bool connectionDatasetWrite, const bool appendToFile, const int traceIterationIndex, const int traceIterationIndexSub, const string connectionDatasetFileNameWrite, const bool connectionTypesDerivedFromPresynapticNeuronsOrEMimages, const bool layerIndexEnforcement, const int layerIndex)
{
	bool result = true;
	
	for(int i=0; i<localConnectomeCSVdatasetConnections->size(); i++)
	{
		vector<string>* localConnectomeConnection = &((*localConnectomeCSVdatasetConnections)[i]);
		
		string connectionSourceNeuronID = (*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_PRE_ID];
		string connectionTargetNeuronID = (*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_POST_ID];
		int connectionExcitationType = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE]);
		
		//cout << "connectionExcitationType = " << connectionExcitationType << endl;
		bool activateConnection = false;
		#ifdef LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES_STILL_REQUIRING_COMPLETION
		if((connectionSourceNeuronID != LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES_UNKNOWN_NEURON_ID) && (connectionTargetNeuronID != LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES_UNKNOWN_NEURON_ID))
		{
		#endif
			int sourceNeuronIndex = (*neuronMap)[connectionSourceNeuronID];
			vector<string>* connectionSourceNeuron = &((*localConnectomeCSVdatasetNeurons)[sourceNeuronIndex]);

			int targetNeuronIndex = (*neuronMap)[connectionTargetNeuronID];
			vector<string>* connectionTargetNeuron = &((*localConnectomeCSVdatasetNeurons)[targetNeuronIndex]);

			#ifdef INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION
			int activationLevelCurrent = SHAREDvars.convertStringToInt((*connectionTargetNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_ACTIVATION_LEVEL_POSITIVE]) - SHAREDvars.convertStringToInt((*connectionTargetNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_ACTIVATION_LEVEL_NEGATIVE]);
			int activationLevelNegativeCurrent = SHAREDvars.convertStringToInt((*connectionTargetNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_ACTIVATION_LEVEL_NEGATIVE]);
			#endif
				
			//target neuron activation;
			int sourceNeuronTraceValue = SHAREDvars.convertStringToInt((*connectionSourceNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE]);
			if(sourceNeuronTraceValue == INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_VALUE_SOURCE)
			{
				#ifdef INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_DRAW_CONNECTIONS_IF_TARGET_ACTIVATED
				if(activationLevelCurrent > INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_ACTIVATION_LEVEL_THRESHOLD_CRITERIA)
				{
				#endif
					if(drawConnectionExcitatory)
					{
						if(connectionExcitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
						{
							activateConnection = true;
						}
					}
					#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_DRAW_INHIBITORY_CONNECTIONS
					if(drawConnectionInhibitory)
					{
						if(connectionExcitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
						{
							activateConnection = true;
						}
					}
					#endif
				#ifdef INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_DRAW_CONNECTIONS_IF_TARGET_ACTIVATED
				}
				#endif
			}

			//connection update;
			int connectionTraceValue = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_TRACE]);	
			int connectionTraceHighlightValue = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT1]);
			int connectionTraceMarkValue = SHAREDvars.convertStringToInt((*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT2]);
			if(activateConnection)
			{
				if(connectionExcitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
				{
					if(drawConnectionExcitatory)
					{
						connectionTraceValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_CONNECTION_TRACE_VALUE_ACTIVE;
					}
					if(highlightConnectionExcitatory)
					{
						connectionTraceHighlightValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_CONNECTION_TRACE_HIGHLIGHT1_VALUE_EXCITATORY;
					}
					if(markConnectionExcitatory)
					{
						connectionTraceMarkValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_CONNECTION_TRACE_HIGHLIGHT2_VALUE_EXCITATORY;
					}
				}			
				#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_DRAW_INHIBITORY_CONNECTIONS
				if(connectionExcitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
				{
					if(drawConnectionInhibitory)
					{
						connectionTraceValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_CONNECTION_TRACE_VALUE_ACTIVE;
					}
					if(highlightConnectionInhibitory)
					{
						connectionTraceHighlightValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_CONNECTION_TRACE_HIGHLIGHT1_VALUE_INHIBITORY;
					}
					if(markConnectionInhibitory)
					{
						connectionTraceMarkValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_CONNECTION_TRACE_HIGHLIGHT2_VALUE_INHIBITORY;
					}
				}						
				#endif
			}
			else	
			{
				//support many-to-one and one-to-many connections (take no action)
				//neuronTraceHighlightValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_VALUE_INACTIVE;
			}
			(*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_TRACE] = SHAREDvars.convertIntToString(connectionTraceValue);	
			(*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT1] = SHAREDvars.convertIntToString(connectionTraceHighlightValue);	
			(*localConnectomeConnection)[LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT2] =  SHAREDvars.convertIntToString(connectionTraceMarkValue);			
		
		
			//target neuron update;
			string neuronType = (*connectionTargetNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE];
			int neuronExcitationType = LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_UNKNOWN;
			if(neuronType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_PYRAMIDAL)
			{
				neuronExcitationType = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY;
			}
			else if(neuronType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_TYPE_INTERNEURON)
			{
				neuronExcitationType = LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY;
			}
			int targetNeuronTraceValue = SHAREDvars.convertStringToInt((*connectionTargetNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE]);
			int neuronTraceHighlightValue = SHAREDvars.convertStringToInt((*connectionTargetNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT1]);
			int neuronTraceMarkValue = SHAREDvars.convertStringToInt((*connectionTargetNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT2]);
			if(activateConnection)
			{
				#ifdef INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_DRAW_CONNECTIONS_IF_SOURCE_ACTIVATED
				if(activationLevelCurrent > INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_ACTIVATION_LEVEL_THRESHOLD_CRITERIA)
				{
				#endif
					if(neuronExcitationType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
					{
						if(drawTargetExcitatory)
						{
							targetNeuronTraceValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_VALUE_TARGET;
						}
						if(highlightTargetExcitatory)
						{
							neuronTraceHighlightValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_HIGHLIGHT1_VALUE_EXCITATORY;
						}
						if(markTargetExcitatory)
						{
							neuronTraceMarkValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_HIGHLIGHT2_VALUE_EXCITATORY;
						}
					}			
					#ifdef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_DRAW_INHIBITORY_NEURONS
					if(neuronExcitationType == LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
					{
						if(drawTargetInhibitory)
						{
							targetNeuronTraceValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_VALUE_TARGET;
						}
						if(highlightTargetInhibitory)
						{
							neuronTraceHighlightValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_HIGHLIGHT1_VALUE_INHIBITORY;
						}
						if(markTargetInhibitory)
						{
							neuronTraceMarkValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_HIGHLIGHT2_VALUE_INHIBITORY;
						}
					}						
					#endif
					if(connectionExcitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_EXCITATORY)
					{	
						if(drawTargetOfExcitatoryConnection)
						{
							targetNeuronTraceValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_VALUE_TARGET;
						}
						if(highlightTargetOfExcitatoryConnection)
						{
							neuronTraceHighlightValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_HIGHLIGHT1_VALUE_EXCITATORY;
						}
						if(markTargetOfExcitatoryConnection)
						{
							neuronTraceMarkValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_HIGHLIGHT2_VALUE_EXCITATORY;
						}
					}
					if(connectionExcitationType == LOCAL_CONNECTOME_DATASET_CONNECTIONS_FIELD_INDEX_EXCITATION_TYPE_INHIBITORY)
					{
						if(drawTargetOfInhibitoryConnection)
						{
							targetNeuronTraceValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_VALUE_TARGET;
						}
						if(highlightTargetOfInhibitoryConnection)
						{
							neuronTraceHighlightValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_HIGHLIGHT1_VALUE_INHIBITORY;
						}
						if(markTargetOfInhibitoryConnection)
						{
							neuronTraceMarkValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_HIGHLIGHT2_VALUE_INHIBITORY;
						}
					}
				#ifdef INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_DRAW_CONNECTIONS_IF_SOURCE_ACTIVATED
				}
				#ifdef INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_HIGHLIGHT2_INHIBITED_NEURONS
				else if(activationLevelCurrent < 0)	//CHECKTHIS	//(activationLevelNegativeCurrent > 0)
				{
					if(markInhibitedNeurons)
					{
						neuronTraceMarkValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_HIGHLIGHT2_VALUE_INHIBITORY;
					}	
				}
				#endif
				#ifdef INDEXED_CSV_DATABASE_TRACE_VISUALISATION_SIMULATION_HIGHLIGHT2_EXCITED_NEURONS
				else if(activationLevelCurrent > 0)	//CHECKTHIS
				{
					if(markExcitedNeurons)
					{
						neuronTraceMarkValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_HIGHLIGHT2_VALUE_EXCITATORY;
					}	
				}
				#endif
				#endif
			}
			else	
			{
				//support many-to-one and one-to-many connections (take no action)
				//neuronTraceHighlightValue = INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_NEURON_TRACE_VALUE_INACTIVE;
			}
			(*connectionTargetNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE] =  SHAREDvars.convertIntToString(targetNeuronTraceValue);
			(*connectionTargetNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT1] =  SHAREDvars.convertIntToString(neuronTraceHighlightValue);	
			(*connectionTargetNeuron)[LOCAL_CONNECTOME_DATASET_NEURONS_FIELD_INDEX_ARTIFICIAL_TRACE_HIGHLIGHT2] =  SHAREDvars.convertIntToString(neuronTraceMarkValue);			
		#ifdef LOCAL_CONNECTOME_DATASET_CONNECTIONS_FILENAME_TYPES_DERIVED_FROM_EM_IMAGES_STILL_REQUIRING_COMPLETION
		}
		#endif
	}	
		
	bool visualiseTrace = true;
	int traceIterationIndexVisualisation = traceIterationIndex*traceIterationIndexSubMax + traceIterationIndexSub;
	
	#ifdef LOCAL_CONNECTOME_VISUALISATION_2D_SVG
	LCindexedCSVdatabaseVisualiseLocalConnectome.visualiseLocalConnectomeCSVdataset(local_connectome_folder_base, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, true, localConnectomeCSVdatasetNeurons, localConnectomeCSVdatasetConnections, visualiseTrace, traceIterationIndexVisualisation);
	#endif
	#ifndef INDEXED_CSV_DATABASE_TRACE_LOCAL_CONNECTOME_EMULATE_PUBLICATION_INFORMATION_FLOW_THROUGH_THE_H01_NETWORK
	#ifdef LOCAL_CONNECTOME_VISUALISATION_3D_LDR
	LCindexedCSVdatabaseVisualiseLocalConnectome.visualiseLocalConnectomeCSVdataset(local_connectome_folder_base, connectionTypesDerivedFromPresynapticNeuronsOrEMimages, false, localConnectomeCSVdatasetNeurons, localConnectomeCSVdatasetConnections, visualiseTrace, traceIterationIndexVisualisation);
	#endif
	#endif
	
	return result;
}

#endif
