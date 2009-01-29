package antipasto.GUI.GadgetListView.GadgetPanelEvents;

import java.util.EventListener;



public interface IActiveGadgetChangedEventListener extends EventListener {
    public void onActiveGadgetChanged(ActiveGadgetObject obj);
}
