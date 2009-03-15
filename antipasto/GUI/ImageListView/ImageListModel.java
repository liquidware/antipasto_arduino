package antipasto.GUI.ImageListView;

import java.awt.List;
import java.io.File;
import java.util.ArrayList;

import javax.swing.ListModel;
import javax.swing.event.EventListenerList;
import javax.swing.event.ListDataEvent;
import javax.swing.event.ListDataListener;
import java.io.*;

public class ImageListModel implements ListModel, IImageFileCollectionChangedListener{

	ImageFileCollection fileCollection = new ImageFileCollection();
	private EventListenerList listDataListener = new EventListenerList();
	
	public ImageListModel(File[] files){
		fileCollection.addItemListener(this);
		for(int i=0; i < files.length; i++){
				fileCollection.add(files[i]);
		}
		fileCollection.addItemListener(this);
	}
	
	public void addListDataListener(ListDataListener l) {
		listDataListener.add(ListDataListener.class, l);
	}

	public Object getElementAt(int index) {
		return fileCollection.get(index);
	}

	public int getSize() {
		return fileCollection.size();
	}

	public void removeListDataListener(ListDataListener l) {
		listDataListener.remove(ListDataListener.class, l);
	}
	
	public void itemsChanged(){
		this.onListDataChanged();
	}
	
	public void remove(int index){
		try{
			File f = (File)fileCollection.get(index);
			if(!f.delete()){
				System.out.println("Can't delete the file, check permissions or delete manuallyL " + f.getPath());
			}else{
				fileCollection.removeFile(index);
			}
		}catch (Exception ex) {
			System.out.println("error removing from fileCollection");
			ex.printStackTrace();
		}
	}
	
	private void onListDataChanged(){
        Object[] listeners = listDataListener.getListenerList();
                    // Each listener occupies two elements - the first is the listener class
                    // and the second is the listener instance
                    for (int i=0; i<listeners.length; i+=2) {
                        if (listeners[i]== ListDataListener.class) {
                            ((ListDataListener)listeners[i+1]).contentsChanged(new ListDataEvent(this, 0,0,0));
                        }
                    }
    }

	public void filesChanged() {
		this.onListDataChanged();
	}

}
