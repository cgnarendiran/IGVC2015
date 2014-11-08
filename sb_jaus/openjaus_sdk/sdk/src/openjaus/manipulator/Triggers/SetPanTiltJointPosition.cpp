
/**
\file SetPanTiltJointPosition.h

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
#include "openjaus/manipulator/Triggers/SetPanTiltJointPosition.h"

namespace openjaus
{
namespace manipulator
{

SetPanTiltJointPosition::SetPanTiltJointPosition() : 
	model::Message(),
	joint1Position_rad(),
	joint2Position_rad()
{
	this->id = SetPanTiltJointPosition::ID; // Initialize id member
	setType(transport::JAUS_MESSAGE);


	fields.push_back(&joint1Position_rad);
	joint1Position_rad.setName("Joint1Position");
	joint1Position_rad.setOptional(false);
	// Nothing to init

	fields.push_back(&joint2Position_rad);
	joint2Position_rad.setName("Joint2Position");
	joint2Position_rad.setOptional(false);
	// Nothing to init

}

SetPanTiltJointPosition::SetPanTiltJointPosition(model::Message *message) :
	model::Message(message),
	joint1Position_rad(),
	joint2Position_rad()
{
	this->id = SetPanTiltJointPosition::ID; // Initialize id member
	setType(transport::JAUS_MESSAGE);


	fields.push_back(&joint1Position_rad);
	joint1Position_rad.setName("Joint1Position");
	joint1Position_rad.setOptional(false);
	// Nothing to init

	fields.push_back(&joint2Position_rad);
	joint2Position_rad.setName("Joint2Position");
	joint2Position_rad.setOptional(false);
	// Nothing to init


	system::Buffer *payloadBuffer = dynamic_cast<system::Buffer *>(message->getPayload());
	if(payloadBuffer)
	{
		this->from(payloadBuffer);
		payloadBuffer->reset();
	}
}

SetPanTiltJointPosition::~SetPanTiltJointPosition()
{

}


double SetPanTiltJointPosition::getJoint1Position_rad(void)
{
	return this->joint1Position_rad.getValue();
}

void SetPanTiltJointPosition::setJoint1Position_rad(double value)
{
	this->joint1Position_rad.setValue(value);
}

double SetPanTiltJointPosition::getJoint2Position_rad(void)
{
	return this->joint2Position_rad.getValue();
}

void SetPanTiltJointPosition::setJoint2Position_rad(double value)
{
	this->joint2Position_rad.setValue(value);
}

int SetPanTiltJointPosition::to(system::Buffer *dst)
{
	int byteSize = dst->pack(this->id);
	byteSize += dst->pack(joint1Position_rad);
	byteSize += dst->pack(joint2Position_rad);
	return byteSize;
}

int SetPanTiltJointPosition::from(system::Buffer *src)
{
	int byteSize = src->unpack(this->id);
	byteSize += src->unpack(joint1Position_rad);
	byteSize += src->unpack(joint2Position_rad);
	return byteSize;
}

int SetPanTiltJointPosition::length(void)
{
	int length = 0;
	length += sizeof(uint16_t); // Message ID
	length += joint1Position_rad.length(); // joint1Position_rad
	length += joint2Position_rad.length(); // joint2Position_rad
	return length;
}

std::string SetPanTiltJointPosition::toXml(unsigned char level) const
{
	std::ostringstream prefix;
	for(unsigned char i = 0; i < level; i++)
	{
		prefix << "\t";
	}

	std::ostringstream oss;
	oss << prefix.str() << "<Message name=\"SetPanTiltJointPosition\"";
	oss << " id=\"0x0622\" >\n";
	oss << joint1Position_rad.toXml(level+1); // joint1Position_rad
	oss << joint2Position_rad.toXml(level+1); // joint2Position_rad
	oss << prefix.str() << "</Message>\n";
	return oss.str();
}

} // namespace manipulator
} // namespace openjaus

