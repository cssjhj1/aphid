/*
 *  HBase.h
 *  masq
 *
 *  Created by jian zhang on 5/4/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include <string>
class Vector3F;
#include <AllHdf.h>
class HBase : public HGroup {
public:
	HBase(const std::string & path);
	virtual ~HBase();
	
	void addIntAttr(const char * attrName);
	void addIntData(const char * dataName, unsigned count);
	void addFloatData(const char * dataName, unsigned count);
	void addVector3Data(const char * dataName, unsigned count);
	void addCharData(const char * dataName, unsigned count);
	
	void writeIntAttr(const char * attrName, int *value);
	void writeIntData(const char * dataName, unsigned count, int *value, HDataset::SelectPart * part = 0);
	void writeFloatData(const char * dataName, unsigned count, float *value, HDataset::SelectPart * part = 0);
	void writeVector3Data(const char * dataName, unsigned count, Vector3F *value, HDataset::SelectPart * part = 0);
	void writeCharData(const char * dataName, unsigned count, char *value, HDataset::SelectPart * part = 0);
	
	char readIntAttr(const char * attrName, int *value);
	char readIntData(const char * dataname, unsigned count, unsigned *dst, HDataset::SelectPart * part = 0);
	char readFloatData(const char * dataname, unsigned count, float *dst, HDataset::SelectPart * part = 0);
	char readVector3Data(const char * dataname, unsigned count, Vector3F *dst, HDataset::SelectPart * part = 0);
	char readCharData(const char * dataName, unsigned count, char *dst, HDataset::SelectPart * part = 0);
	
	char hasNamedAttr(const char * attrName);
	std::string getAttrName(hid_t attrId);
	
	char hasNamedChild(const char * childName);
	std::string getChildName(hsize_t i);
	
	char hasNamedData(const char * dataName);
	char discardNamedAttr(const char * path);
	
	int numChildren();
	int numAttrs();
	
	virtual char save();
	virtual char load();
};