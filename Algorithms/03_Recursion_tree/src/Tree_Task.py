class TreeNode: 
    def __init__(self, info): 
        self.info = info
        self.left = self.right = None

def create_Tree():
    try:
        input_str = input("enter x: ")
        if not input_str: 
            return None
        x = int(input_str)
    except: 
        print("!cin >> x (Ошибка ввода)")
        return None

    root = TreeNode(x)
    root.left = create_Tree()
    root.right = create_Tree()

    return root


# 1
def 





