#include <stdio.h>

#include "layout.h"
//! SHIVA TALWAR PEBBLE CHALLENGE 

//! Initialize node with the given values
//! @param node the node to initialize
//! @param name A name for the node. You do not have to deep copy this value
//! @param id An id for the node
//! @param position A relative position for this node with respect to it's parent
void layout_node_init(struct LayoutNode* node, const char* name, int id, struct Position position)
{
	struct LayoutNode *treeNode = node;
	treeNode->name = name;
	treeNode->id = id;
	treeNode->position = position;
	node->firstChild = NULL;
	node->sibling = NULL;
	node->parent = NULL;

}

//! Add an initialized node as the root of the tree
void layout_init(struct Layout* layout, struct LayoutNode* root)
{
	struct Layout *treeLayout = layout;
	treeLayout->root = root;
	treeLayout->root->firstChild = NULL;
	treeLayout->root->sibling = NULL;
	treeLayout->root->parent = NULL;
}
//! Add an intialized node as the child of another node already in the Layout tree
void layout_add_child(struct Layout* layout, struct LayoutNode* parent, struct LayoutNode* child)
{
	if (parent->firstChild == NULL)
	{
		parent->firstChild = child;
	}
	else
	{
		struct LayoutNode* tmp = parent->firstChild;
		while (tmp->sibling != NULL)
		{
			tmp = tmp->sibling;
		}
		tmp->sibling = child;
	}
	child->parent = parent;
}

//! Change the position of a node. This should work whether or not the node is already in a tree
void layout_node_update_position(struct Layout* layout, struct LayoutNode* node, struct Position position)
{
	node->position = position;
}

//! @return the absolute position for the node with the given memory address
struct Position layout_get_position_for_node(struct Layout* layout, struct LayoutNode* node)
{
	return layout_get_position_for_id(layout, node->id);
}


//! @return the absolute position for the node with the given name
struct Position layout_get_position_for_name(struct Layout* layout, const char* name)
{
	//don't actually modify real layout
	struct Layout temp1 = *layout;
	struct Layout *temp = &temp1;
	if (temp->root == NULL)
	{
		struct Position empty;
		empty.x = NULL; 
		empty.y = NULL;
		return empty;
	}
	if (temp->root->name == name)
	{
		struct LayoutNode* tmp = temp->root;
		struct Position pos = tmp->position;
		while (tmp->parent != NULL)
		{
			tmp = tmp->parent;
			pos.x += tmp->position.x;
			pos.y += tmp->position.y;
		}
		return pos;
	}

	temp->root = temp->root->firstChild;
	struct Position result = layout_get_position_for_name(temp, name);
	if (result.x == NULL)
	{
		if (temp->root != NULL)
		{
			temp->root = temp->root->sibling;
		}
		result = layout_get_position_for_name(temp, name);
	}
	return result;

}

//! @return the absolute position for the node with the given id
struct Position layout_get_position_for_id(struct Layout* layout, int id)
{
	//don't actually modify real layout
	struct Layout temp1 = *layout;
	struct Layout *temp = &temp1;
	if (temp->root == NULL)
	{
		struct Position empty;
		empty.x = NULL;
		empty.y = NULL;
		return empty;
	}
	if (temp->root->id == id)
	{
		struct LayoutNode* tmp = temp->root;
		struct Position pos = tmp->position;
		while (tmp->parent != NULL)
		{
			tmp = tmp->parent;
			pos.x += tmp->position.x;
			pos.y += tmp->position.y;
		}
		return pos;
	}

	temp->root = temp->root->firstChild;
	struct Position result = layout_get_position_for_id(temp, id);
	if (result.x == NULL)
	{
		if (temp->root != NULL)
		{
			temp->root = temp->root->sibling;
		}
		result = layout_get_position_for_id(temp, id);
	}
	return result;

}


int main(int argc, char** argv) {
    struct Layout layout;

    struct LayoutNode root_node;
    struct LayoutNode node_a, node_b;
    struct LayoutNode node_aa;

    layout_node_init(&root_node, "r", 0, (struct Position) { .x = 10, .y = 10 });
    layout_init(&layout, &root_node);

    layout_node_init(&node_a, "a", 1, (struct Position) { .x = 10, .y = 10 });
    layout_add_child(&layout, &root_node, &node_a);

    layout_node_init(&node_b, "b", 2, (struct Position) { .x = 20, .y = 40 });
    layout_add_child(&layout, &root_node, &node_b);

    layout_node_init(&node_aa, "aa", 3, (struct Position) { .x = 20, .y = 40 });
    layout_add_child(&layout, &node_a, &node_aa);

    struct Position pos_a = layout_get_position_for_node(&layout, &node_a);
    printf("Node a should be (20, 20), got (%u, %u)\n", pos_a.x, pos_a.y);

    struct Position pos_b = layout_get_position_for_name(&layout, "b");
    printf("Node b should be (30, 50), got (%u, %u)\n", pos_b.x, pos_b.y);

    struct Position pos_aa = layout_get_position_for_id(&layout, 3);
    printf("Node aa should be (40, 60), got (%u, %u)\n", pos_aa.x, pos_aa.y);

    layout_node_update_position(&layout, &node_a, (struct Position) { .x = 10, .y = 20 });

    pos_a = layout_get_position_for_node(&layout, &node_a);
    printf("Node a should be (20, 30), got (%u, %u)\n", pos_a.x, pos_a.y);

    pos_b = layout_get_position_for_name(&layout, "b");
    printf("Node b should be (30, 50), got (%u, %u)\n", pos_b.x, pos_b.y);

    pos_aa = layout_get_position_for_id(&layout, 3);
    printf("Node aa should be (40, 70), got (%u, %u)\n", pos_aa.x, pos_aa.y);
}

