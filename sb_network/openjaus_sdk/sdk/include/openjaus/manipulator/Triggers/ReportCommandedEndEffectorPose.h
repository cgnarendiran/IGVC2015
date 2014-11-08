
/**
\file ReportCommandedEndEffectorPose.h

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

#ifndef REPORTCOMMANDEDENDEFFECTORPOSE_H
#define REPORTCOMMANDEDENDEFFECTORPOSE_H

#include <openjaus.h>

#include "openjaus/manipulator/Triggers/Fields/ToolPointCoordinateXScaledInteger.h"
#include "openjaus/manipulator/Triggers/Fields/ToolPointCoordinateYScaledInteger.h"
#include "openjaus/manipulator/Triggers/Fields/ToolPointCoordinateZScaledInteger.h"
#include "openjaus/manipulator/Triggers/Fields/DComponentOfUnitQuaternionQScaledInteger.h"
#include "openjaus/manipulator/Triggers/Fields/AComponentOfUnitQuaternionQScaledInteger.h"
#include "openjaus/manipulator/Triggers/Fields/BComponentOfUnitQuaternionQScaledInteger.h"
#include "openjaus/manipulator/Triggers/Fields/CComponentOfUnitQuaternionQScaledInteger.h"

namespace openjaus
{
namespace manipulator
{

/// \class ReportCommandedEndEffectorPose ReportCommandedEndEffectorPose.h
/// \brief ReportCommandedEndEffectorPose Message Implementation.
/// This message provides the receiver with the commanded pose of the end effector.
class OPENJAUS_EXPORT ReportCommandedEndEffectorPose : public model::Message
{
public:
	ReportCommandedEndEffectorPose();
	ReportCommandedEndEffectorPose(model::Message *message);
	~ReportCommandedEndEffectorPose();
	
	static const uint16_t ID = 0x4610;

	/// \brief Pack this message to the given openjaus::system::Buffer. 
	/// \copybrief
	/// \param[out] dst - The destination openjaus::system::Buffer to which this message will be packed.
	/// \return The number of bytes packed into the destination buffer
	virtual int to(system::Buffer *dst);	

	/// \brief Unpack this message from the given openjaus::system::Buffer.
	/// \copybrief
	/// \param[in] src - The source openjaus::system::Buffer from which this message will be unpacked.
	/// \return The number of bytes unpacked from the source buffer
	virtual int from(system::Buffer *src);

	/// \brief Get the number of bytes this message would occupy in a serialized buffer. 
	/// \copybrief
	/// \return The number of bytes this message would occupy in a buffer
	virtual int length();
	
	/// \brief Used to serialize the runtime state of the message to an XML format. 
	/// \copybrief
	/// \param[in] level - Used to determine how many tabs are inserted per line for pretty formating. 
	/// \return The serialized XML string
	std::string toXml(unsigned char level=0) const;



	double getToolPointCoordinateX_m(void);
	void setToolPointCoordinateX_m(double value);

	double getToolPointCoordinateY_m(void);
	void setToolPointCoordinateY_m(double value);

	double getToolPointCoordinateZ_m(void);
	void setToolPointCoordinateZ_m(double value);

	double getDComponentOfUnitQuaternionQ(void);
	void setDComponentOfUnitQuaternionQ(double value);

	double getAComponentOfUnitQuaternionQ(void);
	void setAComponentOfUnitQuaternionQ(double value);

	double getBComponentOfUnitQuaternionQ(void);
	void setBComponentOfUnitQuaternionQ(double value);

	double getCComponentOfUnitQuaternionQ(void);
	void setCComponentOfUnitQuaternionQ(double value);

private:
	ToolPointCoordinateXScaledInteger toolPointCoordinateX_m;
	ToolPointCoordinateYScaledInteger toolPointCoordinateY_m;
	ToolPointCoordinateZScaledInteger toolPointCoordinateZ_m;
	DComponentOfUnitQuaternionQScaledInteger dComponentOfUnitQuaternionQ;
	AComponentOfUnitQuaternionQScaledInteger aComponentOfUnitQuaternionQ;
	BComponentOfUnitQuaternionQScaledInteger bComponentOfUnitQuaternionQ;
	CComponentOfUnitQuaternionQScaledInteger cComponentOfUnitQuaternionQ;

};

} // namespace manipulator
} // namespace openjaus

#endif // REPORTCOMMANDEDENDEFFECTORPOSE_H

