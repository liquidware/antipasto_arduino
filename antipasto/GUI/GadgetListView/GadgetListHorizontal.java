package antipasto.GUI.GadgetListView;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.io.File;
import java.util.ArrayList;

import javax.swing.DefaultListModel;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

import processing.app.Base;

import antipasto.GadgetFactory;
import antipasto.ModuleFactory;
import antipasto.Interfaces.IModule;

public class GadgetListHorizontal extends JPanel implements ISelectedItemListener{
	
	File baseDirectory;
	ArrayList modules = new ArrayList();
	JScrollPane scrollPane = new JScrollPane();
	GadgetList gadgetList;
	public GadgetListHorizontal(File gadgetLibDirectory, GadgetList gl){
		super();
		
		gadgetList = gl;
		baseDirectory = gadgetLibDirectory;
		
		scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
		scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_NEVER);
		scrollPane.setSize(new Dimension(300, 60));
		scrollPane.setPreferredSize(new Dimension(300, 60));
		scrollPane.setBorder(null);
		
		this.add(scrollPane);
		this.loadDirectory();
		
		scrollPane.setVisible(true);
	}

	/**
	 * Sets the internal gadget
	 * @param list Gadgetlist that we can add library modules to
	 */
	public void setGadgetList(GadgetList list){
		this.gadgetList = list;
	}
	
	private void loadDirectory(){
		File[] files = this.baseDirectory.listFiles();
		ModuleFactory fact = new ModuleFactory();
		JPanel itemsPanel = new JPanel();
		itemsPanel.setBackground(Color.WHITE);
		itemsPanel.setLayout(new FlowLayout());
		itemsPanel.setSize(new Dimension(this.scrollPane.getWidth(), 70 ));
		File libDir = Base.createTempFolder("moduleLib");
		for(int i = 0; i < files.length; i ++){
			if(files[i].getName().endsWith(IModule.moduleExtension)){
				try {
					//File newFile = new File(libDir + File.separator + files[i].getName());
					//Base.copyFile(files[i], newFile);
					File curFile = files[i];
					IModule mod = fact.loadModule(curFile, libDir.getPath(), false);
					//ImageIcon icon = new ImageIcon(mod.getImage());
					ModuleIcon label = new ModuleIcon(mod, this.gadgetList);
					label.setSize(60,60);
					label.setPreferredSize(new Dimension(60,60));
					label.setVisible(true);
					label.addSelectionListener(this);
					itemsPanel.add(label);
					//scrollPane.add(label);
					modules.add(label);
					System.out.println(mod.getName());
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
		scrollPane.setSize(new Dimension(300, 60));
		scrollPane.setPreferredSize(new Dimension(300, 60));
		scrollPane.setViewportView(itemsPanel);
		itemsPanel.setVisible(true);		
	}

	public void onSelected(ModuleIcon selectedItem) {
		if(gadgetList.gadget != null){
			GadgetFactory fact = new GadgetFactory();
			try {
				System.out.println("Attempting to add and reload the gadget");
				fact.AddModuleToGadget(gadgetList.gadget, selectedItem.getModule());
				gadgetList.saveCurrentGadget();
				gadgetList.loadGadget(gadgetList.gadget);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
}
