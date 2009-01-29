package antipasto.GUI.GadgetListView.GadgetPanelEvents;


import java.util.EventListener;

import antipasto.GUI.GadgetListView.GadgetCollection;

public interface IGadgetCollectionChangedListener extends EventListener {
    public void OnGadgetCollectionChanged(GadgetCollection gc);
}
