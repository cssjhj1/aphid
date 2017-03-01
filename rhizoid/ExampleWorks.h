/*
 *  ExampleWorks.h
 *  proxyPaint
 *
 *  select viz
 *  query and edit example parameters
 *
 *  Created by jian zhang on 3/1/17.
 *  Copyright 2017 __MyCompanyName__. All rights reserved.
 *
 */
 
 #ifndef RHI_EXAMPLE_WORKS_H
 #define RHI_EXAMPLE_WORKS_H
 
 #include "ProxyVizNode.h"
 
 class MSelectionList;
 
 class ExampleWorks {
 
 public:
	ExampleWorks();
	virtual ~ExampleWorks();
	
	MString getExampleStatusStr();
	
 protected:
 /// active viz
	static aphid::ProxyViz * PtrViz;
	static MObject ObjViz;
	
	bool validateViz(const MSelectionList &sels);
	bool validateSelection();
	
 private:
 };
 
 #endif
 

