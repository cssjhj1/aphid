/*
 *  SampleFrame.cpp
 *  opium
 *
 *  Created by jian zhang on 12/23/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#include "SampleFrame.h"
#include <sstream>
#include <iostream>

SampleFrame::SampleFrame() : m_spf(1) {}

void SampleFrame::setFirst(const int& iframe, const int& isample, 
	                const float& weight)
{
    m_frames[0] = iframe;
    m_samples[0] = isample;
    m_weights[0] = weight;
}

void SampleFrame::calculateWeights(double frameTime, const int & spf)
{
    std::cout<<"\n SampleFrame::calculateWeights "<<frameTime;
    frameTime = double(int(frameTime * 1000 + 0.5))/1000;
    m_samples[0] = m_samples[1] = 0;
	const double subframe = frameTime - (int)frameTime;
	if(spf == 1) {
	    if(frameTime >= 0.0) {
	        m_frames[0] = (int)frameTime;
		} else {
		    m_frames[0] = (int)frameTime - 1;
		}
		m_frames[1] = m_frames[0] + 1;
		m_weights[0] = 1.0 - subframe;
		m_weights[1] = 1.0 - m_weights[0];
	}
	else {
		const double delta = 1.0 / spf;
		double minTime, maxTime;
		if(frameTime >= 0.0) {
		    m_frames[0] = m_frames[1] = (int)frameTime;
		} else {
		    m_frames[0] = m_frames[1] = (int)frameTime - 1;
		}
/// in between
		for(int i = 0; i < spf; i++) {
			minTime = delta * i;
			maxTime = minTime + delta;
			if(minTime <= subframe && maxTime > subframe) {
				m_samples[0] = i;
				m_samples[1] = i + 1;
				m_weights[0] = 1.0 - (subframe - minTime) / delta;
				m_weights[1] = 1.0 - m_weights[0];
			}
		}
		if(m_samples[1] == spf) {
			m_frames[1] = m_frames[0] + 1;
			m_samples[1] = 0;
		}
	}
}

int SampleFrame::sampleOfset0() const
{ return (m_frames[0] - m_minFrame) * m_spf + m_samples[0]; }
	
int SampleFrame::sampleOfset1() const
{ return (m_frames[1] - m_minFrame) * m_spf + m_samples[1]; }

std::string SampleFrame::str() const
{
    std::stringstream sst;
    sst<<"\n sample frame:"
    <<"spf "<<m_spf
        <<"\n frame[2] "<<m_frames[0]<<", "<<m_frames[1]
        <<"\n sample[2] "<<m_samples[0]<<", "<<m_samples[1]
        <<"\n weight[2] "<<m_weights[0]<<", "<<m_weights[1];
    return sst.str();
}
	
