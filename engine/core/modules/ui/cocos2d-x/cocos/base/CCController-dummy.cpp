#include "base/CCController.h"
#include "nau/diag/assertion.h"

NS_CC_BEGIN

Controller::Controller()
	: _controllerTag(TAG_UNSET)
	, _impl(nullptr)
	, _connectEvent(nullptr)
	, _keyEvent(nullptr)
	  , _axisEvent(nullptr)
{
	init();
}

Controller::~Controller()
{
    NAU_ASSERT(_connectEvent == nullptr);
    NAU_ASSERT(_keyEvent == nullptr);
    NAU_ASSERT(_axisEvent == nullptr);
}


NS_CC_END
