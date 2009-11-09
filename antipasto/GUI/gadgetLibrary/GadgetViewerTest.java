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
		
		frame.setVisible(true);
		
	}
}
