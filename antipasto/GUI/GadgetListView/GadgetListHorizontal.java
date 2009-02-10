package antipasto.GUI.GadgetListView;

import java.awt.Color;
import java.awt.Dimension;
import java.io.File;
import java.util.ArrayList;

import javax.swing.DefaultListModel;
import javax.swing.ImageIcon;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;

import antipasto.ModuleFactory;
import antipasto.Interfaces.IModule;

public class GadgetListHorizontal extends JPanel implements ISelectedItemListener{
	
	File baseDirectory;
	ArrayList modules = new ArrayList();
	JScrollPane scrollPane = new JScrollPane();
	public GadgetListHorizontal(File gadgetLibDirectory){
		super();
		baseDirectory = gadgetLibDirectory;
		//list.setModel(model);
		//list.setModel(new GadgetListModel(_collection));
		//list.setCellRenderer(new LibraryGadgetCellRenderer());
		//this.loadDirectory();
		//list.setSize(80, 250);
		//list.setFixedCellHeight(80);
		//list.setFixedCellWidth(80);
		//JScrollPane scrollPane = new JScrollPane(list);
		//scrollPane.setSize(this.getSize());
		//this.add(scrollPane);
		//this.add(list);
		//list.setVisible(true);
		//list.setAutoscrolls(true);
		//list.setLayoutOrientation(JList.HORIZONTAL_WRAP);
		scrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
		scrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_NEVER);
		scrollPane.setSize(new Dimension(250, 83));
		scrollPane.setPreferredSize(new Dimension(250, 83));
		this.add(scrollPane);
		this.loadDirectory();
	}
	
	private void loadDirectory(){
		File[] files = this.baseDirectory.listFiles();
		ModuleFactory fact = new ModuleFactory();
		for(int i = 0; i < files.length; i ++){
			if(files[i].getName().endsWith(IModule.moduleExtension)){
				try {
					IModule mod = fact.loadModule(files[i], System.getProperty("java.io.tmpdir"), false);
					//ImageIcon icon = new ImageIcon(mod.getImage());
					ModuleIcon label = new ModuleIcon(mod);
					label.setSize(83,83);
					label.setPreferredSize(new Dimension(83,83));
					label.setVisible(true);
					label.addSelectionListener(this);
					this.scrollPane.add(label);
					modules.add(label);
					System.out.println(mod.getName());
				} catch (Exception e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
	}

	public void onSelected(ModuleIcon selectedItem) {
		System.out.println("Changing selected item to :" + selectedItem.getModule().getName());
		for(int i = 0; i < modules.size(); i++){
			System.out.println(((ModuleIcon)modules.get(i)).getModule().getName());
			if(selectedItem.equals(modules.get(i))){
				selectedItem.setBackground(Color.blue);
				selectedItem.setOpaque(true);
			}else{
				((ModuleIcon)modules.get(i)).setBackground(Color.white);
				selectedItem.setOpaque(false);
			}
		}
		this.repaint();
	}
}
