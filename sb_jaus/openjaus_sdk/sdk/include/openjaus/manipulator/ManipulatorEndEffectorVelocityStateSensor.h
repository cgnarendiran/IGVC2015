/**
\file ManipulatorEndEffectorVelocityStateSensor.h

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

#ifndef MANIPULATORENDEFFECTORVELOCITYSTATESENSOR_COMPONENT_H
#define MANIPULATORENDEFFECTORVELOCITYSTATESENSOR_COMPONENT_H

#include <openjaus.h>
#include "openjaus/core/Base.h"
#include "openjaus/manipulator/ManipulatorEndEffectorVelocityStateSensorInterface.h"
#include "openjaus/manipulator/Transitions/EndEffectorVelocityStateSensorDefaultLoop.h"
#include "openjaus/manipulator/Transitions/EndEffectorVelocityStateSensorControlledLoop.h"
#include "openjaus/manipulator/Triggers/QueryManipulatorSpecifications.h"
#include "openjaus/manipulator/Triggers/QueryToolOffset.h"
#include "openjaus/manipulator/Triggers/SetToolOffset.h"
#include "openjaus/manipulator/Triggers/QueryEndEffectorVelocityState.h"
#include "openjaus/manipulator/Triggers/ReportManipulatorSpecifications.h"
#include "openjaus/manipulator/Triggers/ReportToolOffset.h"
#include "openjaus/manipulator/Triggers/ReportEndEffectorVelocityState.h"
// Start of user code for additional headers:
// End of user code

namespace openjaus
{
namespace manipulator
{

/// \class ManipulatorEndEffectorVelocityStateSensor ManipulatorEndEffectorVelocityStateSensor.h
/// \brief %ManipulatorEndEffectorVelocityStateSensor Component implements the urn:jaus:jss:manipulator:ManipulatorEndEffectorVelocityStateSensor services.
/// The %ManipulatorEndEffectorVelocityStateSensor component provides an implementation of the following service(s). This component can 
/// be extended for customized functionality or used via instantiation.
/// <dl>
/// <dt>%ManipulatorEndEffectorVelocityStateSensor Service</dt>
/// <dd><p>
/// The function of the End Effector Velocity State Sensor is to report the velocity state of the tool tip as defined by
/// two length-three vectors, i.e. ωe and vtool,e. These vectors respectively represent the angular velocity of the
/// end-effector coordinate system and the linear velocity of the tool tip as measured with respect to the manipulator
/// base system.
/// </p><br/><br/>
/// <b>URI:</b> urn:jaus:jss:manipulator:ManipulatorEndEffectorVelocityStateSensor<br/><br/>
/// <b>Version:</b> 1.0<br/>
/// <dl><dt><b>Inherits From:</b></dt>
/// </dl></dd>
/// </dl>
class OPENJAUS_EXPORT ManipulatorEndEffectorVelocityStateSensor : public core::Base, public manipulator::ManipulatorEndEffectorVelocityStateSensorInterface
{

public:
	ManipulatorEndEffectorVelocityStateSensor();
	virtual ~ManipulatorEndEffectorVelocityStateSensor();

	/// \brief Set the location of the tool point as measured in the end effector coordinate system.
	/// Set the location of the tool point as measured in the end effector coordinate system.
	/// \param[in]  setToolOffset - Input Trigger.
	/// \return Whether the message was properly processed by this action. 
    virtual bool setToolOffset(SetToolOffset *setToolOffset);

	/// \brief Send a report manipulator specs message
	/// Send a report manipulator specs message
	/// \param[in] queryManipulatorSpecifications - Input Trigger.
	/// \return ReportManipulatorSpecifications Output Message.
	virtual ReportManipulatorSpecifications getReportManipulatorSpecifications(QueryManipulatorSpecifications *queryManipulatorSpecifications);

	/// \brief Send a report tool offset message
	/// Send a report tool offset message
	/// \param[in] queryToolOffset - Input Trigger.
	/// \return ReportToolOffset Output Message.
	virtual ReportToolOffset getReportToolOffset(QueryToolOffset *queryToolOffset);

	/// \brief Send a report end effector velocity state message
	/// Send a report end effector velocity state message
	/// \param[in] queryEndEffectorVelocityState - Input Trigger.
	/// \return ReportEndEffectorVelocityState Output Message.
	virtual ReportEndEffectorVelocityState getReportEndEffectorVelocityState(QueryEndEffectorVelocityState *queryEndEffectorVelocityState);


	/// \brief isControllingEndEffectorVelocitySensorClient condition.
	/// isControllingEndEffectorVelocitySensorClient condition.
	/// \param[in]  setToolOffset - Input Trigger.
	/// \return Whether the condition is true. 
	virtual bool isControllingEndEffectorVelocitySensorClient(SetToolOffset *setToolOffset);


	// Start of user code for additional methods:
	// End of user code

protected:
	EndEffectorVelocityStateSensorDefaultLoop endEffectorVelocityStateSensorDefaultLoop;
	EndEffectorVelocityStateSensorControlledLoop endEffectorVelocityStateSensorControlledLoop;


	// Start of user code for additional members:
	// End of user code
};

} // namespace component
} // namespace openjaus

#endif // MANIPULATORENDEFFECTORVELOCITYSTATESENSOR_H