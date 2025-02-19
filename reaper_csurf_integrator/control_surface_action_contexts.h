//
//  control_surface_base_actions.h
//  reaper_csurf_integrator
//
//

#ifndef control_surface_action_contexts_h
#define control_surface_action_contexts_h

#include "control_surface_integrator.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class NoAction : public Action
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
public:
    virtual string GetName() override { return "NoAction"; }
    
    virtual void RequestUpdate(ActionContext* context) override
    {
        context->UpdateColorValue(0.0);
        context->ClearWidget();
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ReaperAction : public Action
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
public:
    virtual string GetName() override { return "ReaperAction"; }
   
    virtual void RequestUpdate(ActionContext* context) override
    {
        int state = DAW::GetToggleCommandState(context->GetCommandId());
        
        if(state == -1) // this Action does not report state
            state = 0;
        
        if( ! (context->GetRangeMinimum() == -2.0 || context->GetRangeMaximum() == 2.0)) // used for Increase/Decrease
            context->UpdateWidgetValue(state);
    }
    
    virtual void Do(ActionContext* context, double value) override
    {
        // used for Increase/Decrease
        if(value < 0 && context->GetRangeMinimum() < 0)
            DAW::SendCommandMessage(context->GetCommandId());
        else if(value > 0 && context->GetRangeMinimum() >= 0)
            DAW::SendCommandMessage(context->GetCommandId());
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FXAction : public Action
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
public:
    virtual string GetName() override { return "FXAction"; }
    
    virtual double GetCurrentNormalizedValue(ActionContext* context) override
    {
        if(MediaTrack* track = context->GetTrack())
        {
            double min, max = 0.0;
            
            return DAW::TrackFX_GetParam(track, context->GetSlotIndex(), context->GetParamIndex(), &min, &max);
        }
        else
            return 0.0;
    }

    virtual void RequestUpdate(ActionContext* context) override
    {
        if(MediaTrack* track = context->GetTrack())
        {
            double currentValue = GetCurrentNormalizedValue(context);
            
            context->UpdateWidgetValue(currentValue);
        }
        else
            context->ClearWidget();
    }
};

#endif /* control_surface_action_contexts_h */
