package antipasto.Util;


import org.w3c.dom.Element;

import antipasto.Interfaces.IVersionable;

/**
 * Created by IntelliJ IDEA.
 * User: Omar
 */
public class XMLVersionReader{
    public static IVersionable Load(Element versionNode) throws Exception {
        IVersionable version ;
        if(versionNode.getNodeName().equalsIgnoreCase("version"))
        {
            int majorVersion = Integer.parseInt(versionNode.getAttribute("Major"));
            int minorVersion = Integer.parseInt(versionNode.getAttribute("Minor"));
            int revision = Integer.parseInt(versionNode.getAttribute("Revision"));
            version = new Version(majorVersion, minorVersion, revision);

            return version;
        }
        else
        {
            throw new Exception("There was an error loading the xml");
        }
    }
}
