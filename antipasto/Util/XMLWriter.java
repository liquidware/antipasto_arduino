package antipasto.Util;

import org.w3c.dom.Element;


import javax.xml.transform.TransformerException;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.Transformer;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.dom.DOMSource;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.logging.Level;
import java.util.logging.Logger;

public class XMLWriter {
    public static File Writer(Element elements, String fileName, String outputDirectory) throws Exception
    {
        try {

            
            Transformer transformer = TransformerFactory.newInstance().newTransformer();
            transformer.setOutputProperty(OutputKeys.INDENT, "yes");

            File f = new File(outputDirectory + File.separator + fileName);

            StreamResult result = new StreamResult(f);
            DOMSource source = new DOMSource(elements);
            transformer.transform(source, result);
            return f;
        } catch (TransformerException ex) {
          Logger.getLogger(XMLWriter.class.getName()).log(Level.SEVERE, null, ex);
        }
        throw new Exception("Error while creating XML file");
    }

    private static File WriteFile(String xml, File f)
    {
        BufferedWriter out = null;
        try {
            out = new BufferedWriter(new FileWriter(f.getCanonicalPath(), true));
            out.write(xml);
            out.close();
            return f;
        } catch (IOException ex) {
            Logger.getLogger(XMLWriter.class.getName()).log(Level.SEVERE, null, ex);
        } finally {
            try {
                out.close();
            } catch (IOException ex) {
                Logger.getLogger(XMLWriter.class.getName()).log(Level.SEVERE, null, ex);
            }
        }

        return null;
    }
}
