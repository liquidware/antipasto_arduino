package antipasto.Util;

import org.w3c.dom.Document;
import org.w3c.dom.Element;

import antipasto.Interfaces.IVersionable;


public class XMLVersionCreator {
    public static Element CreateVersionElement(Document doc, IVersionable version)
    {
        Element retVersion = doc.createElement("Version");
        retVersion.setAttribute("Major", String.valueOf(version.getMajorNumber()));
        retVersion.setAttribute("Minor", String.valueOf(version.getMinorNumber()));
        retVersion.setAttribute("Revision", String.valueOf(version.getRevisionNumber()));
        return retVersion;
    }
}
