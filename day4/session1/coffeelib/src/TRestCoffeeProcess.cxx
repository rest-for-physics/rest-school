/*************************************************************************
 * This file is part of the REST software framework.                     *
 *                                                                       *
 * Copyright (C) 2016 GIFNA/TREX (University of Zaragoza)                *
 * For more information see http://gifna.unizar.es/trex                  *
 *                                                                       *
 * REST is free software: you can redistribute it and/or modify          *
 * it under the terms of the GNU General Public License as published by  *
 * the Free Software Foundation, either version 3 of the License, or     *
 * (at your option) any later version.                                   *
 *                                                                       *
 * REST is distributed in the hope that it will be useful,               *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the          *
 * GNU General Public License for more details.                          *
 *                                                                       *
 * You should have a copy of the GNU General Public License along with   *
 * REST in $REST_PATH/LICENSE.                                           *
 * If not, see http://www.gnu.org/licenses/.                             *
 * For the list of contributors see $REST_PATH/CREDITS.                  *
 *************************************************************************/

/////////////////////////////////////////////////////////////////////////
/// Write the process description Here                                   
/// 
/// ### Parameters
/// Describe any parameters this process receives: 
/// * **parameter1**: This parameter ...
/// * **parameter2**: This parameter is ...
/// 
/// 
/// ### Examples
/// Give examples of usage and RML descriptions that can be tested.      
/// \code
///     <WRITE A CODE EXAMPLE HERE>
/// \endcode
/// 
/// ### Running pipeline example
/// Add the examples to a pipeline to guarantee the code will be running 
/// on future framework upgrades.                                        
/// 
/// 
/// Please, add any figure that may help to ilustrate the process        
/// 
/// \htmlonly <style>div.image img[src="trigger.png"]{width:500px;}</style> \endhtmlonly
/// ![An ilustration of the trigger definition](trigger.png)             
/// 
/// The png image should be uploaded to the ./images/ directory          
///                                                                      
///----------------------------------------------------------------------
///                                                                      
/// REST-for-Physics - Software for Rare Event Searches Toolkit 		    
///                                                                      
/// History of developments:                                             
///                                                                      
/// YEAR-Month: First implementation of TRestCoffeeProcess
/// WRITE YOUR FULL NAME 
///                                                                      
/// \class TRestCoffeeProcess                                               
/// \author: TODO. Write full name and e-mail:        nkx
///                                                                      
/// <hr>                                                                 
///                                                                      

#include "TRestCoffeeProcess.h"

ClassImp(TRestCoffeeProcess);

///////////////////////////////////////////////                          
/// \brief Default constructor                                          
///                                                                      
TRestCoffeeProcess::TRestCoffeeProcess() {
    Initialize();
}

///////////////////////////////////////////////                          
/// \brief Default destructor                                           
///                                                                      
TRestCoffeeProcess::~TRestCoffeeProcess() {
}

///////////////////////////////////////////////                          
/// \brief Function to initialize input/output event members and define  
/// the section name                                                     
///                                                                      
void TRestCoffeeProcess::Initialize() {
    SetSectionName(this->ClassName());
    SetLibraryVersion(LIBRARY_VERSION);
    fAnaEvent = NULL;

    // Initialize here the values of class data members if needed       

}

///////////////////////////////////////////////                           
/// \brief Process initialization. Observable names can be re-interpreted 
/// here. Any action in the process required before starting event process 
/// might be added here.                                                 
///                                                                      
void TRestCoffeeProcess::InitProcess() {
    // Write here the jobs to do before processing
    // i.e., initialize histograms and auxiliary vectors,
    // read TRestRun metadata, or load additional rml sections

}

///////////////////////////////////////////////                          
/// \brief The main processing event function                           
///                                                                      
TRestEvent* TRestCoffeeProcess::ProcessEvent(TRestEvent * evInput) {
    fAnaEvent = (TRestRawSignalEvent*)evInput;

    // Write here the main logic of process: TRestCoffeeProcess
    // Read data from input event, write data to output event, and save observables to tree

    return fAnaEvent;
}

///////////////////////////////////////////////                          
/// \brief Function to include required actions after all events have been
/// processed.                                                            
///                                                                       
void TRestCoffeeProcess::EndProcess() {
    // Write here the jobs to do when all the events are processed

}

