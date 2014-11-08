
/**
\file ReportCommandedPanTiltJointVelocity.h

\par Copyright
Copyright (c) 2012, OpenJAUS, LLC
All rights reserved.

This file is part of the OpenJAUS Software Development Kit (SDK). This 
software is distributed under one of two licenses, the OpenJAUS SDK 
Commercial End User License Agreement or the OpenJAUS SDK Non-Commercial 
End User License Agreement. The appropriate licensing details were included 
in with your developer credentials and software download. See the LICENSE 
file included with this software for full details.
 
THIS SOFTWARE IS PROVIDED BY THE LICENSOR (OPENJAUS LCC) "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE LICENSOR BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
OUT OF THE USE OF THE SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
DAMAGE. THE LICENSOR DOES NOT WARRANT THAT THE LICENSED SOFTWARE WILL MEET
LICENSEE'S REQUIREMENTS OR THAT THE OPERATION OF THE LICENSED SOFTWARE
WILL BE UNINTERRUPTED OR ERROR-FREE, OR THAT ERRORS IN THE LICENSED
SOFTWARE WILL BE CORRECTED.

\ Software History
- [2011-08-23] - Added AS6057: Manipulators
- [2011-08-01] - Added AS6060: Environment Sensing
- [2011-06-16] - First Release 

*/

#include <openjaus.h>
#include "openjaus/manipulator/Triggers/ReportCommandedPanTiltJointVelocity.h"

namespace openjaus
{
namespace manipulator
{

ReportCommandedPanTiltJointVelocity::ReportCommandedPanTiltJointVelocity() : 
	model::Message(),
	joint1Velocity_rps(),
	joint2Velocity_rps()
{
	this->id = ReportCommandedPanTiltJointVelocity::ID; // Initialize id member
	setType(transport::JAUS_MESSAGE);


	fields.push_back(&joint1Velocity_rps);
	joint1Velocity_rps.setName("Joint1Velocity");
	joint1Velocity_rps.setOptional(false);
	// Nothing to init

	fields.push_back(&joint2Velocity_rps);
	joint2Velocity_rps.setName("Joint2Velocity");
	joint2Velocity_rps.setOptional(false);
	// Nothing to init

}

ReportCommandedPanTiltJointVelocity::ReportCommandedPanTiltJointVelocity(model::Message *message) :
	model::Message(message),
	joint1Velocity_rps(),
	joint2Velocity_rps()
{
	this->id = ReportCommandedPanTiltJointVelocity::ID; // Initialize id member
	setType(transport::JAUS_MESSAGE);


	fields.push_back(&joint1Velocity_rps);
	joint1Velocity_rps.setName("Joint1Velocity");
	joint1Velocity_rps.setOptional(false);
	// Nothing to init

	fields.push_back(&joint2Velocity_rps);
	joint2Velocity_rps.setName("Joint2Velocity");
	joint2Velocity_rps.setOptional(false);
	// Nothing to init


	system::Buffer *payloadBuffer = dynamic_cast<system::Buffer *>(message->getPayload());
	if(payloadBuffer)
	{
		this->from(payloadBuffer);
		payloadBuffer->reset();
	}
}

ReportCommandedPanTiltJointVelocity::~ReportCommandedPanTiltJointVelocity()
{

}


double ReportCommandedPanTiltJointVelocity::getJoint1Velocity_rps(void)
{
	return this->joint1Velocity_rps.getValue();
}

void ReportCommandedPanTiltJointVelocity::setJoint1Velocity_rps(double value)
{
	this->joint1Velocity_rps.setValue(value);
}

double ReportCommandedPanTiltJointVelocity::getJoint2Velocity_rps(void)
{
	return this->joint2Velocity_rps.getValue();
}

void ReportCommandedPanTiltJointVelocity::setJoint2Velocity_rps(double value)
{
	this->joint2Velocity_rps.setValue(value);
}

int ReportCommandedPanTiltJointVelocity::to(system::Buffer *dst)
{
	int byteSize = dst->pack(this->id);
	byteSize += dst->pack(joint1Velocity_rps);
	byteSize += dst->pack(joint2Velocity_rps);
	return byteSize;
}

int ReportCommandedPanTiltJointVelocity::from(system::Buffer *src)
{
	int byteSize = src->unpack(this->id);
	byteSize += src->unpack(joint1Velocity_rps);
	byteSize += src->unpack(joint2Velocity_rps);
	return byteSize;
}

int ReportCommandedPanTiltJointVelocity::length(void)
{
	int length = 0;
	length += sizeof(uint16_t); // Message ID
	length += joint1Velocity_rps.length(); // joint1Velocity_rps
	length += joint2Velocity_rps.length(); // joint2Velocity_rps
	return length;
}

std::string ReportCommandedPanTiltJointVelocity::toXml(unsigned char level) const
{
	std::ostringstream prefix;
	for(unsigned char i = 0; i < level; i++)
	{
		prefix << "\t";
	}

	std::ostringstream oss;
	oss << prefix.str() << "<Message name=\"ReportCommandedPanTiltJointVelocity\"";
	oss << " id=\"0x4631\" >\n";
	oss << joint1Velocity_rps.toXml(level+1); // joint1Velocity_rps
	oss << joint2Velocity_rps.toXml(level+1); // joint2Velocity_rps
	oss << prefix.str() << "</Message>\n";
	return oss.str();
}

} // namespace manipulator
} // namespace openjaus

