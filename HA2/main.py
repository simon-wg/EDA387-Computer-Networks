import sys
import random
from enum import Enum
from typing import Any, Self, override

NUM_PROCESSORS = 10


class Color(Enum):
    BLUE = 0
    RED = 1
    GREEN = 2


class Processor:
    color: Color
    index: int

    def __init__(self, index: int, color: Color = Color.BLUE):
        self.index = index
        self.color = color

    def action(self, other: Self):
        self.color = Color((other.color.value + 1) % 2)

    @override
    def __str__(self):
        return f"p{self.index}: x{self.index}={self.color}"


def print_state(processors: list[Processor]):
    [print(p) for p in processors]


def all_x_in_y(x: list[Any], y: list[Any]) -> bool:
    for item in x:
        if item not in y:
            return False
    return True


def action_loop(processors: list[Processor]) -> int:
    if len(processors) == 1:
        return 0
    curr_async_rounds = 0
    executed_processors = []
    processor_indices = list(range(len(processors)))
    while True:
        curr = random.choice(processors)
        prev = processors[curr.index - 1]
        executed_processors.append(curr.index)
        if curr.index == 0:
            curr.color = Color.GREEN
            continue
        curr.action(prev)
        if all(p in executed_processors for p in processor_indices):
            curr_async_rounds += 1
            executed_processors = []
        if check_neighbors(processors):
            return curr_async_rounds


def check_neighbors(processors: list[Processor]) -> bool:
    for i in range(len(processors)):
        prev = processors[i - 1]
        curr = processors[i]
        next = processors[(i + 1) % len(processors)]
        if (curr.color == prev.color) or (curr.color == next.color):
            return False
    return True


def main():
    args = sys.argv
    number_of_processors = 2
    try:
        number_of_processors = int(args[1])
    except:
        pass
    processors = [Processor(i) for i in range(number_of_processors)]
    rounds_taken = action_loop(processors)
    print(f"Stable in {rounds_taken} rounds")


if __name__ == "__main__":
    main()
