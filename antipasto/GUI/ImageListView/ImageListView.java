package antipasto.GUI.ImageListView;

import java.awt.Color;
import java.io.File;
import java.util.ArrayList;

import javax.swing.JList;

import antipasto.GUI.GadgetListView.GadgetPanel;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.ActiveGadgetObject;
import antipasto.GUI.GadgetListView.GadgetPanelEvents.IActiveGadgetChangedEventListener;
import antipasto.Interfaces.IGadget;
import antipasto.Interfaces.IModule;


public class ImageListView extends JList implements IActiveGadgetChangedEventListener {
	ArrayList imageFiles = new ArrayList();
	IModule activeModule;
	GadgetPanel gadgetPanel;
	public ImageListView(GadgetPanel panel){
		activeModule = panel.getActiveModule();
		panel.addActiveGadgetChangedEventListener(this);
		gadgetPanel = panel;
		if(activeModule != null){
			this.loadList(activeModule.getData());
		}
	}
	
	public void loadList(File[] files){
		this.setCellRenderer(new ImageCellRenderer());
		ImageListModel listModel = new ImageListModel(files);
		this.setModel(listModel);
	}

	public void onActiveGadgetChanged(ActiveGadgetObject obj) {
		this.activeModule = gadgetPanel.getActiveModule();
		this.loadList(this.activeModule.getData());
	}
}
