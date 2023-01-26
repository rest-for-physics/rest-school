/*************************************************************************
 * This file is part of the REST software framework.                     *
 *                                                                       *
 * Copyright (C) 2016 GIFNA/TREX (University of Zaragoza)                *
 * For more information see https://gifna.unizar.es/trex                 *
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
 * If not, see https://www.gnu.org/licenses/.                            *
 * For the list of contributors see $REST_PATH/CREDITS.                  *
 *************************************************************************/

#ifndef REST_TRestCoffee
#define REST_TRestCoffee

#include "TRestMetadata.h"

/// UPDATE Write here a brief description. Just one line!
class TRestCoffee : public TRestMetadata {
private:

    // REMOVE COMMENT. Add here the members/parameters for your metadata class.
    // You can set their default values here together. 
    // Note: add "//!" mark at the end of the member definition
    // if you don't want to save them to disk. The following dummy member are given as examples.

    /// REMOVE MEMBER. A dummy member that will be written to the ROOT file.
    Double_t fDummy = 3.14; //<

    /// REMOVE MEMBER. A dummy member that will be NOT written to the ROOT file.
    Double_t fDummyVar = 3.14; //!

    void Initialize() override;

public:
    /// UPDATE Documentation of dummy getter
    Double_t GetDummy() { return fDummy;}

    /// UPDATE Documentation of dummy getter
    Double_t GetDummyVar() { return fDummy;}

    /// UPDATE Documentation of dummy setter
    void SetDummy( const Double_t &dummy) { fDummy = dummy;}

    /// UPDATE Documentation of dummy setter
    void SetDummyVar( const Double_t &dummy) { fDummyVar = dummy;}

public:
    void PrintMetadata() override;

    TRestCoffee();
    TRestCoffee(const char* configFilename, std::string name = "");
    ~TRestCoffee();

    // REMOVE COMMENT. ROOT class definition helper. Increase the number in it every time
    // you add/rename/remove the metadata members
    ClassDefOverride(TRestCoffee, 1);

};
#endif
