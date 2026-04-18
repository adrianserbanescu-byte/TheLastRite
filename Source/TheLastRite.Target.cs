using UnrealBuildTool;
using System.Collections.Generic;

public class TheLastRiteTarget : TargetRules
{
    public TheLastRiteTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
        ExtraModuleNames.Add("TheLastRite");
    }
}
