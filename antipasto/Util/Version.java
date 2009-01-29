package antipasto.Util;

import antipasto.Interfaces.IVersionable;

/**
 * Created by IntelliJ IDEA.
 * User: Omar
 */
public class Version implements IVersionable {
    int _major, _minor, _revision;

    public Version(int major, int minor, int revision)
    {
        _major = major;
        _minor = minor;
        _revision = revision;
    }

    public int getMajorNumber() {
        return _major;
    }

    public int getMinorNumber() {
        return _minor;
    }

    public int getRevisionNumber() {
        return _revision;
    }

    public void setMajorNumber(int value) {
        _major = value;
    }

    public void setMinorNumber(int value) {
        _minor = value;
    }

    public void setRevisionNumber(int value) {
        _revision = value;
    }
}
