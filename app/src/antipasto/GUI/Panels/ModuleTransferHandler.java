package antipasto.GUI.Panels;

import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.File;
import java.io.IOException;
import java.util.List;

import javax.swing.JComponent;
import javax.swing.TransferHandler;

import antipasto.GadgetFactory;
import antipasto.ModuleFactory;
import antipasto.Interfaces.IModule;

public class ModuleTransferHandler extends TransferHandler{
	public IModule[] lastTransferedModules;
	public boolean importData(JComponent c, Transferable t)
	{
		DataFlavor[] flavors = t.getTransferDataFlavors();
		DataFlavor fileFlavor = DataFlavor.javaFileListFlavor;
		java.util.List modules = new java.util.ArrayList();
			try {
				java.util.List files =
                    (java.util.List)t.getTransferData(fileFlavor);
				
				for(int i1=0; i1 < files.size(); i1++){
					File f = (File) files.get(i1);
					if(f.getName().endsWith(".module")){
						modules.add(f);
					}
				}
			} catch (UnsupportedFlavorException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}
			
			IModule[] savedModule = new IModule[modules.size()];
			
			ModuleFactory fact = new ModuleFactory();
			for(int i = 0; i < modules.size(); i++){
				try {
					savedModule[i] = fact.loadModule((File)modules.get(i), System.getProperty("java.io.tmpdir") + File.separator + (((File)(modules.get(i))).getName()),false);
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
			
			if(savedModule.length > 0)
			{
				//create a new sketch
				GadgetFactory gadgFact = new GadgetFactory();
				try {
					System.out.println("********************************");
					System.out.println("creating a new gadget");
					gadgFact.CreateGadgetFile("Test", "C:\\openhardware", savedModule);
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		return true;
	}
}
