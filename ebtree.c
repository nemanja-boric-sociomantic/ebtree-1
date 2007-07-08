/*
 * Elastic Binary Trees
 * (C) 2002-2007 - Willy Tarreau <w@1wt.eu>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "ebtree.h"

int eb_delete(struct eb_node *node) {
    return __eb_delete(node);
}

struct eb32_node *eb32_insert(struct eb32_node *root, struct eb32_node *new) {
    return __eb32_insert(root, new);
}

struct eb32_node *eb32_lookup(struct eb32_node *root, unsigned long x) {
    return __eb32_lookup(root, x);
}

struct eb64_node *eb64_insert(struct eb64_node *root, struct eb64_node *new) {
    return __eb64_insert(root, new);
}

/* Returns first leaf in the tree starting at <root>, or NULL if none */
struct eb_node *
eb_first_node(struct eb_node *root)
{
	return __eb_first_node(root);
}

#if 0
// Alternative method, may clobber less registers.
/* Returns first leaf in the tree starting at <root>, or NULL if none */
struct eb_node *
eb_first_node(struct eb_node *root)
{
	return eb_walk_down_left((struct eb_node *)(root),
		((struct eb_node *)(root))->leaf[0] ?
		    ((struct eb_node *)(root))->leaf[0] :
		    ((struct eb_node *)(root))->leaf[1]);
}
#endif

/* returns last leaf in the tree starting at <root>, or NULL if none */
struct eb_node *
eb_last_node(struct eb_node *root)
{
	return __eb_last_node(root);
}

/* returns previous leaf node before an existing leaf node, or NULL if none. */
struct eb_node *
eb_prev_node(struct eb_node *node)
{
	if (node->dup.p != &node->dup) {
		/* let's return duplicates before going further */
		node = LIST_ELEM(node->dup.p, struct eb_node *, dup);
		if (unlikely(!node->leaf_p))
			return node;
		/* we returned to the list's head, let's walk up now */
	}
	node = eb_walk_up_left_with_parent(node, node->leaf_p);
	if (node)
		node = eb_walk_down_right(node, node->leaf[0]);
	return node;
}

/* returns next leaf node after an existing leaf node, or NULL if none. */
struct eb_node *
eb_next_node(struct eb_node *node)
{
	if (node->dup.n != &node->dup) {
		/* let's return duplicates before going further */
		node = LIST_ELEM(node->dup.n, struct eb_node *, dup);
		if (unlikely(!node->leaf_p))
			return node;
		/* we returned to the list's head, let's walk up now */
	}
	node = eb_walk_up_right_with_parent(node, node->leaf_p);
	if (node)
		node = eb_walk_down_left(node, node->leaf[1]);
	return node;
}

