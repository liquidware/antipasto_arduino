package antipasto.Util;

import antipasto.Interfaces.IVersionable;

/**
 * Created by IntelliJ IDEA.
 * User: Omar
 */
public class VersionCompatiablity {
    public static boolean IsSameOrNewer(IVersionable requiredVersion, IVersionable actualVersion)
    {
        if(requiredVersion.getMajorNumber() > actualVersion.getMajorNumber())
        {
            return false;
        }
        else if(requiredVersion.getMajorNumber() < actualVersion.getMajorNumber())
        {
            //this version is newer than the old one so we can just dump true
            return true;
        }

        if(requiredVersion.getMinorNumber() > actualVersion.getMinorNumber())
        {
            return false;
        }
        else if(requiredVersion.getMinorNumber() < actualVersion.getMinorNumber())
        {
            //this version is newer than the old one so we can just dump true
            return true;
        }

        if(requiredVersion.getRevisionNumber() > actualVersion.getRevisionNumber())
        {
            return false;
        }
        else if(requiredVersion.getRevisionNumber() < actualVersion.getRevisionNumber())
        {
            //this version is newer than the old one so we can just dump true
            return true;
        }

        return true;           //should never hit this...but just in case
    }
}
