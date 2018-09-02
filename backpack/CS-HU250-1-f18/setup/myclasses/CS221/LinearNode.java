/**
 * LinearNode represents a node in a linked list.
 *
 * @author Java Foundations, mvail
 * @version 4.0
 */
public class LinearNode<E> {

	private LinearNode<E> next;
	private LinearNode<E> previous;
	private E element;

	/**
  	 * Creates an empty node.
  	 */
	public LinearNode() {
		next = null;
		previous=null;
		element = null;
		
	}

	/**
  	 * Creates a node storing the specified element.
 	 *
  	 * @param elem
  	 *            the element to be stored within the new node
  	 */
	public LinearNode(E elem) {
		next = null;
		previous=null;
		element = elem;
	}

	/**
 	 * Returns the node that follows this one.
  	 *
  	 * @return the node that follows the current one
  	 */
	public LinearNode<E> getNext() {
		return next;
	}

	/**
 	 * Sets the node that follows this one.
 	 *
 	 * @param node
 	 *            the node to be set to follow the current one
 	 */
	public void setNext(LinearNode<E> node) {
		next = node;
	}

	/**
 	 * Returns the element stored in this node.
 	 *
 	 * @return the element stored in this node
 	 */
	public E getElement() {
		return element;
	}

	/**
 	 * Sets the element stored in this node.
  	 *
  	 * @param elem
  	 *            the element to be stored in this node
  	 */
	public void setElement(E elem) {
		element = elem;
	}
	/**
 	 * Returns the element stored in the previous node.
 	 *
 	 * @return the element stored in the previous node
 	 */

	public LinearNode<E> getPrevious() {
		return previous;
	}
	
	/**
 	 * Sets the element stored in the previous node.
  	 *
  	 * @param elem
  	 *            the element to be stored in the previous node
  	 */
	
	public void setPrevious(LinearNode<E> previous) {
		this.previous = previous;
	}
	
	@Override
	public String toString() {
		return "Element: " + element.toString() + " Has next: " + (next != null);
	}
}

