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
		scrollPane.setSize(new Dimension(300, 90));
		scrollPane.setPreferredSize(new Dimension(300, 90));
		
		this.add(scrollPane);
		this.loadDirectory();
		
		scrollPane.setVisible(true);
	}
	
	private void loadDirectory(){
		File[] files = this.baseDirectory.listFiles();
		ModuleFactory fact = new ModuleFactory();
		JPanel itemsPanel = new JPanel();
		itemsPanel.setBackground(Color.WHITE);
		itemsPanel.setLayout(new FlowLayout());
		itemsPanel.setSize(new Dimension(this.scrollPane.getWidth(), 90 ));
		for(int i = 0; i < files.length; i ++){
			if(files[i].getName().endsWith(IModule.moduleExtension)){
				try {
					IModule mod = fact.loadModule(files[i], System.getProperty("java.io.tmpdir"), false);
					//ImageIcon icon = new ImageIcon(mod.getImage());
					ModuleIcon label = new ModuleIcon(mod, this.gadgetList);
					label.setSize(83,83);
					label.setPreferredSize(new Dimension(83,83));
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
		//this.scrollPane = new JScrollPane(itemsPanel, JScrollPane.VERTICAL_SCROLLBAR_NEVER, JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
		scrollPane.setSize(new Dimension(300, 83));
		scrollPane.setPreferredSize(new Dimension(300, 83));
		scrollPane.setViewportView(itemsPanel);
		//scrollPane.add(itemsPanel);
		//this.add(itemsPanel);
		itemsPanel.setVisible(true);		
	}

	public void onSelected(ModuleIcon selectedItem) {
		/*System.out.println("Changing selected item to :" + selectedItem.getModule().getName());
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
		this.repaint();*/
	}
}
