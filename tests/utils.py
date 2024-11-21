import random as _random


class Random(_random.Random):
    """Utility to generate random items."""

    def __init__(self, x: str = __name__):
        super().__init__(x)

    def int(self, start: int = 1000, stop: int = 2000) -> int:
        """
        Generate a random integer in an interval.

        :param start: Left end of the interval (inclusive).
        :param stop: Right end of the interval (exclusive).

        :return: Random integer.
        """
        return super().randrange(start, stop)
