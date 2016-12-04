2-3-4-Trees

Haskell program with self balancing tree. There are 3 types of nodes.

Nodes that contain one data item and have two subtrees, called a 2-node. Every thing in the left subtree is less than or equal to the node's data item. Everything in the right subtree is greater than the node's data item.

Nodes that contain two data item and have three subtrees, called a 3-node. Every thing in the left subtree is less than or equal to the node's first data item. Everything in the middle subtree is greater than the node's first data item and less than or equal to the node's second data item. Everything in the right subtree is greater than the node's second data item.

Nodes that contain three data item and have four subtrees, called a 4-node. Every thing in the first (left) subtree is less than or equal to the node's first data item. Everything in the second subtree is greater than the node's first data item and less than or equal to the node's second data item. Everything in the third subtree is greater than the node's second data item and less than or equal to the node's third data item. Everything in the fourth (right) subtree is greater than the node's third data item.

The rules for insertion are

Find the lowest node it can be added into. This will modify the node the data was added to: This will convert a 2-node into a 3-node. This will convert a 3-node into a 4-node. In the case of a 4-node, take the middle data item and add it to the node's parent. This will modify the parent node: If the parent node is a 2-node, it will covert into a 3-node. If the parent node is a 3-node, it will covert into a 4-node. If the parent node is a 4-node, then take the middle data element and promote it to its parent. If the parent is the root and it is a 4-node, split it into a 2-node with two 3-node children.