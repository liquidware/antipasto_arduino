package antipasto.GUI.gadgetLibrary;

import java.io.File;

import javax.swing.JFrame;

import antipasto.ModuleFactory;
import antipasto.GUI.GadgetListView.GadgetListHorizontal;
import antipasto.Interfaces.IModule;

public class GadgetViewerTest {
	public static void main(String args[]){
		JFrame frame = new JFrame();
		frame.setSize(300, 200);
		
		File gadgetLibraryDirectory = new File("C:" + File.separator + "OpenHardware" + File.separator + "Modules");
		
		/*GadgetHorizontalScrollPane scrollPane = new GadgetHorizontalScrollPane();
		scrollPane.setSize(frame.getSize());
		
		File[] gadgetLibraryFiles = gadgetLibraryDirectory.listFiles();
		ModuleFactory factory = new ModuleFactory();
				
		for(int i=0; i <gadgetLibraryFiles.length; i++){
			if(gadgetLibraryFiles[i].getName().toLowerCase().endsWith(IModule.moduleExtension.toLowerCase())){
				String gadgetDirectory = gadgetLibraryDirectory.getPath() + File.separator 
										 + gadgetLibraryFiles[i].getName().replace(".module", "");
				try {
					IModule gadget = factory.loadModule(gadgetLibraryFiles[i], gadgetDirectory , false);
					scrollPane.add(gadget);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}*/
		
		//GadgetListHorizontal glh = new GadgetListHorizontal(gadgetLibraryDirectory);
		
		//frame.add(glh);
		frame.setVisible(true);
		
	}
}
