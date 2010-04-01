package antipasto.Plugins;

import processing.app.Editor;

public class EditorContext {
	private Editor editor = null;
	
	public EditorContext(Editor editor){
		setEditor(editor);
	}
	public Editor getEditor(){
		return this.editor;
	}
	public EditorContext setEditor(Editor e){
		this.editor = e;
		return this;
	}

}
