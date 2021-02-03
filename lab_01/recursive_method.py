def _rec(origin, l1, target, l2):
    if not l1:
        return l2
    elif not l2:
        return l1
            
    a1 = _rec(origin, l1-1, target, l2) + 1
    a2 = _rec(origin, l1, target, l2-1) + 1
    a3 = _rec(origin, l1-1, target, l2-1) + \
        (origin[l1 - 1] != target[l2 - 1])

    return min(a1, a2, a3)

def levenstein_r(origin, target):
    l1 = len(origin)
    l2 = len(target)
    return _rec(origin, l1, target, l2)
