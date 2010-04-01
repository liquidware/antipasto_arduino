package antipasto.ModuleRules;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import antipasto.CoreFactory;


public class ModuleRulesParser {
	private ModuleRules moduleRules = new ModuleRules();
	
	public ModuleRulesParser(File xmlRulesFile){
		try {
			Element root = getRootElement(xmlRulesFile);
			this.parse(root);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public ModuleRules getModuleRules(){
		return moduleRules;
	}
	
	private void parse(Element root){
		ArrayList messages = new ArrayList();
		NodeList children = root.getChildNodes();
		int nodeCount = children.getLength();

		for(int i=0; i<nodeCount; i++){
			Node elem = children.item(i);
			if(elem instanceof Element){
				if(elem.getNodeName().equalsIgnoreCase("message")){
					messages.add(this.messageGenerator((Element) elem));
				}
			}
		}
		this.moduleRules = new ModuleRules();
		this.moduleRules.setMessages(messages);
	}
	
	private IMessage messageGenerator(Element messageElem){
		String message = messageElem.getAttribute("text");
		return new Message(message);
	}
	
	private Element getRootElement(File xmlConfig) throws Exception {
        try {
            DocumentBuilderFactory docFact = DocumentBuilderFactory.newInstance();
            DocumentBuilder docBuilder = docFact.newDocumentBuilder();
            Document doc = docBuilder.parse(xmlConfig);
            return (Element) doc.getFirstChild();
        } catch (SAXException ex) {
            Logger.getLogger(CoreFactory.class.getName()).log(Level.SEVERE, null, ex);
        } catch (IOException ex) {
            Logger.getLogger(CoreFactory.class.getName()).log(Level.SEVERE, null, ex);
        }

        throw new Exception("Error getting the base element of the project");
    }
}
