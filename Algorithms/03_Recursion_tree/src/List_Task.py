class Elem:
    """Класс, представляющий узел (элемент) связного списка."""
    def __init__(self, info):
        self.info = info  # Данные узла
        self.next = None  # Ссылка на следующий узел

def create_new_elem(x):
    """Создает новый узел. Аналог create_new_elem в C++."""
    return Elem(x)

def create_and_enter_list():
    """Создает связный список, считывая данные от пользователя."""
    top = None
    cur = None

    try:
        n_str = input("enter size of list: ")
        if not n_str:
             # Обработка пустого ввода
             return None
        n = int(n_str)
    except ValueError:
        print("!cin >> n (Ошибка ввода)")
        return None

    print(f"enter {n} elem list:")
    
    for _ in range(n):
        try:
            x_str = input()
            if not x_str:
                # Если ввели пустую строку, останавливаем ввод
                break
            x = int(x_str)
        except ValueError:
            print("!cin >> x (Ошибка ввода)")
            # Возвращаем текущий список, как в вашем C++ коде
            return top

        new_elem = create_new_elem(x)

        if top is None:
            top = new_elem
            cur = top
        else:
            cur.next = new_elem
            cur = new_elem

    return top

def output_list(top):
    """Выводит список в консоль."""
    cur = top
    while cur:
        print(cur.info, end=" ")
        cur = cur.next
    print()

# def delete_list(top):
#     """В Python явное удаление не нужно, достаточно сбросить ссылку,
#     сборщик мусора сделает остальное.
#     """
#     # Этот код не нужен в Python, но оставлен для демонстрации
#     # "удаления" - сброса ссылок.
#     # Фактически, достаточно просто выйти из функции, и список
#     # будет удален, если на него нет других ссылок.
#     pass

# --- Операции со списком (Рекурсивные функции) ---

# 1
def number_of_occurr(cur, x, count=0):
    """Рекурсивно считает количество вхождений x."""
    if cur is None:
        return count

    if cur.info == x:
        return number_of_occurr(cur.next, x, count + 1)
    else:
        return number_of_occurr(cur.next, x, count)

# 2
def delete_first_occurr_helper(top, x):
    """
    Вспомогательная функция, которая возвращает новую голову списка
    после удаления первого вхождения x.
    (В Python нельзя передать указатель по ссылке как 'Elem*& top').
    """
    if top is None:
        return None

    if top.info == x:
        # Узел удален (фактически, просто возвращаем следующий узел)
        # Сборщик мусора Python позаботится о старом узле
        return top.next

    # Рекурсивно вызываем для остальной части списка
    top.next = delete_first_occurr_helper(top.next, x)
    return top

# 3
def adding_after_each_el(cur, x, y):
    """Рекурсивно добавляет узел y после каждого вхождения x."""
    if cur is None:
        return

    if cur.info == x:
        new_y = create_new_elem(y)
        new_y.next = cur.next
        cur.next = new_y
        
        # Переходим к узлу, который следует за новым узлом (new_y.next)
        adding_after_each_el(new_y.next, x, y)
    else:
        adding_after_each_el(cur.next, x, y)

# 4
def building_a_copy(other_cur):
    """Рекурсивно строит копию списка и возвращает его голову."""
    if other_cur is None:
        return None

    new_node = Elem(other_cur.info)
    new_node.next = building_a_copy(other_cur.next)
    return new_node

# 5
def are_the_two_lists_equal(cur, other_cur):
    """Рекурсивно проверяет равенство двух списков."""
    if cur is None and other_cur is None:
        return True
    if cur is None or other_cur is None:
        return False
    if cur.info != other_cur.info:
        return False
    
    return are_the_two_lists_equal(cur.next, other_cur.next)

# 6
def found_max_in_list(cur, current_max):
    """Рекурсивно находит максимальный элемент."""
    if cur is None:
        return current_max

    if cur.info > current_max:
        current_max = cur.info
        
    return found_max_in_list(cur.next, current_max)

# 7
def print_list_reverse(cur):
    """Рекурсивно выводит список в обратном порядке."""
    if cur is None:
        return

    print_list_reverse(cur.next)
    print(cur.info, end=" ")

# 8
def delete_list_rec(cur):
    """
    В Python этот код не нужен, сборщик мусора позаботится об очистке
    памяти, когда все ссылки будут удалены. Здесь оставлен для
    демонстрации "рекурсивной очистки", которая по сути лишь
    удаляет ссылки.
    """
    if cur is None:
        return None

    # Рекурсивно удаляем оставшуюся часть списка
    delete_list_rec(cur.next)
    
    # Удаляем ссылку на текущий узел (не удаляет объект, а уменьшает счетчик ссылок)
    cur = None
    return None

# 9
def creating_ord_list_w_destr(cur1, cur2):
    """
    Объединяет два упорядоченных списка, используя узлы
    исходных списков (без создания новых узлов - 'w_destr').
    """
    if cur1 is None: return cur2
    if cur2 is None: return cur1

    if cur1.info < cur2.info:
        cur1.next = creating_ord_list_w_destr(cur1.next, cur2)
        return cur1
    else:
        cur2.next = creating_ord_list_w_destr(cur1, cur2.next)
        return cur2

# 10
def creating_ord_list(cur1, cur2):
    """Объединяет два упорядоченных списка, создавая новый список."""
    # Базовые случаи
    if cur1 is None and cur2 is None:
        return None
    
    # Рекурсивный шаг
    new_node = None
    if cur1 is None:
        new_node = Elem(cur2.info)
        new_node.next = creating_ord_list(cur1, cur2.next)
    elif cur2 is None:
        new_node = Elem(cur1.info)
        new_node.next = creating_ord_list(cur1.next, cur2)
    elif cur1.info >= cur2.info:
        new_node = Elem(cur2.info)
        new_node.next = creating_ord_list(cur1, cur2.next)
    else:
        new_node = Elem(cur1.info)
        new_node.next = creating_ord_list(cur1.next, cur2)
        
    return new_node


# 11
def intersection_ord_lists(cur1, cur2):
    """Рекурсивно находит пересечение двух упорядоченных списков."""
    if cur1 is None or cur2 is None:
        return None

    if cur1.info == cur2.info:
        # Элемент найден, создаем новый узел
        new_node = Elem(cur1.info)
        new_node.next = intersection_ord_lists(cur1.next, cur2.next)
        return new_node
    
    elif cur1.info < cur2.info:
        # cur1 меньше, пропускаем его
        return intersection_ord_lists(cur1.next, cur2)
    else: 
        # cur2 меньше, пропускаем его
        return intersection_ord_lists(cur1, cur2.next)

def main():
    """Основная функция программы."""
    top = create_and_enter_list()
    
    if top is None:
        print("Список пуст или произошла ошибка ввода.")
        return 0

    try:
        choice = int(input("enter the choice: "))
    except ValueError:
        print("!(cin >> choice) (Ошибка ввода)")
        # В Python нет необходимости возвращать код ошибки, как в C++
        return 0

    # В Python часто используют if/elif/else вместо switch/case
    if choice == 1:
        try:
            x = int(input("enter x: "))
        except ValueError:
            print("!(cin >> x) (Ошибка ввода)")
            return 0
        
        num = number_of_occurr(top, x) # count по умолчанию = 0
        print(f"num of occurr {x}: {num}")

    elif choice == 2:
        try:
            x = int(input("enter x to delete: "))
        except ValueError:
            print("!(cin >> x) (Ошибка ввода)")
            return 0
            
        # Обновляем голову списка, используя возвращаемое значение
        new_top = delete_first_occurr_helper(top, x)
        output_list(new_top)
        # Важно: теперь top ссылается на старую голову, а new_top на новую.
        # Для продолжения работы в main нужно обновить top
        top = new_top

    elif choice == 3:
        try:
            x = int(input("enter x: "))
            y = int(input("enter y: "))
        except ValueError:
            print("!(cin >> x) || !(cin >> y) (Ошибка ввода)")
            return 0

        adding_after_each_el(top, x, y)
        output_list(top)

    elif choice == 4:
        new_list = building_a_copy(top)
        output_list(new_list)
        # В Python достаточно удалить ссылку на new_list, 
        # сборщик мусора очистит память.
        new_list = None

    elif choice == 5:
        new_list = create_and_enter_list()
        print("yes" if are_the_two_lists_equal(top, new_list) else "no")
        new_list = None

    elif choice == 6:
        if top is None:
            print("Список пуст, нет максимума.")
            return 0
            
        # Передаем первое значение в качестве начального максимума
        max_val = found_max_in_list(top.next, top.info)
        print(f"Max = {max_val}")

    elif choice == 7:
        print_list_reverse(top)
        print() # Для новой строки

    elif choice == 8:
        # Фактическое удаление в Python
        print("Список удален (ссылка сброшена).")
        # top = None
        # Но чтобы продолжить работу с top в конце main, не сбрасываем здесь.
        pass

    elif choice == 9:
        print("Введите второй упорядоченный список:")
        L2 = create_and_enter_list()
        if L2:
            # L3 становится новой головой объединенного списка, используя узлы top и L2
            L3 = creating_ord_list_w_destr(top, L2)
            output_list(L3)
            # top и L2 теперь являются частью L3, явного delete_list(L2) не нужно.
            # L3 = None
            # Для демонстрации top = L3, L2 = None
            top = L3 
            L2 = None
        else:
            # Если L2 пуст, top остается неизменным
            output_list(top)

    elif choice == 10:
        print("Введите второй упорядоченный список:")
        L2 = create_and_enter_list()
        if L2:
            L3 = creating_ord_list(top, L2)
            output_list(L3)
            # В Python достаточно удалить ссылки
            L3 = None
            L2 = None
        else:
            output_list(top)

    elif choice == 11:
        print("Введите второй упорядоченный список:")
        L2 = create_and_enter_list()
        if L2:
            L3 = intersection_ord_lists(top, L2)
            output_list(L3)
            # В Python достаточно удалить ссылки
            L3 = None
            L2 = None
        else:
             output_list(top)

    # Удаляем ссылку на основной список
    # delete_list(top) # Явный вызов не нужен
    top = None
    return 0

if __name__ == "__main__":
    main()