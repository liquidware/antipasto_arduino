package antipasto.GUI.ImageListView;

import java.util.ArrayList;
import java.util.Collection;
import java.util.EventListener;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;

import javax.swing.event.EventListenerList;

import antipasto.GUI.GadgetListView.GadgetPanelEvents.IGadgetCollectionChangedListener;


public class ImageFileCollection extends ArrayList{
	
	private EventListenerList listDataListener = new EventListenerList();
	public boolean add(Object item){
		boolean returnVal = super.add(item);
		onItemChanged();
		return returnVal;
	}
	
	
	
	public Object getItemAt(int index){
		return super.get(index);
	}
	
	public Object removeFile(int index){
		Object retItem = null;
		try{
			System.out.println("Attempting to get object @ index " + index);
			retItem = getItemAt(index);
			System.out.println("Calling super function to remove object");
			super.remove(getItemAt(index));
			onItemChanged();
		}catch(Exception ex){
			System.out.println("Error removing in imagefilecollection.java");
			ex.printStackTrace();
		}
		return retItem;
	}
	
	public void addItemListener(IImageFileCollectionChangedListener model){
		this.listDataListener.add(IImageFileCollectionChangedListener.class, model);
	}
	
	
	private void onItemChanged(){
		System.out.println("Items changed event lisnteners");
		Object[] listeners = listDataListener.getListenerList();
        // Each listener occupies two elements - the first is the listener class
        // and the second is the listener instance
        for (int i=0; i<listeners.length; i+=2) {
            if (listeners[i]== IGadgetCollectionChangedListener.class) {
                ((IImageFileCollectionChangedListener)listeners[i+1]).filesChanged();
            }
        }
	}
}
