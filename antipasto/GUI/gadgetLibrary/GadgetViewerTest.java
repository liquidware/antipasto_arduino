package antipasto.GUI.gadgetLibrary;

import java.io.File;

import javax.swing.JFrame;

import antipasto.ModuleFactory;
import antipasto.Interfaces.IModule;

public class GadgetViewerTest {
	public static void main(String args[]){
		JFrame frame = new JFrame();
		frame.setSize(300, 200);
		
		GadgetHorizontalScrollPane scrollPane = new GadgetHorizontalScrollPane();
		scrollPane.setSize(frame.getSize());
		
		File gadgetLibraryDirectory = new File("C:\\temp\\Gadgets");
		
		File[] gadgetLibraryFiles = gadgetLibraryDirectory.listFiles();
		ModuleFactory factory = new ModuleFactory();
				
		for(int i=0; i <gadgetLibraryFiles.length; i++){
			if(gadgetLibraryFiles[i].getName().toLowerCase().endsWith(IModule.moduleExtension.toLowerCase())){
				String gadgetDirectory = gadgetLibraryDirectory.getPath() + File.separator 
										 + gadgetLibraryFiles[i].getName().replace(".gadget", "");
				try {
					IModule gadget = factory.loadModule(gadgetLibraryFiles[i], gadgetDirectory , false);
					scrollPane.add(gadget);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
		
		
		frame.add(scrollPane);
		frame.setVisible(true);
		
	}
}
