#!/usr/bin/env python

import unittest

import challenge.basics
import challenge.containers


class DoodadSetTestCase(unittest.TestCase):
    """Test case for for Doodad bindings.
    """

    def compare_Doodads(self, a, b, msg=None):
        if a.name != b.name or a.value != b.value:
            if msg is not None:
                template = "({a.name}, {a.value}) != ({b.name}, {b.value})"
                msg = template.format(a=a, b=b)
            self.failureException(msg)

    def __init__(self, *args, **kwds):
        super(DoodadSetTestCase, self).__init__(*args, **kwds)
        self.addTypeEqualityFunc(challenge.basics.MutableDoodad, self.compare_Doodads)

    def setUp(self):
        self.item0 = challenge.basics.MutableDoodad("a", 0)
        self.item1 = challenge.basics.MutableDoodad("b", 1)
        self.container = challenge.containers.DoodadSet()
        self.container.add(self.item0)
        self.container.add(self.item1)

    def test_as_list(self):
        """Test converting the DoodadSet to list.
        """
        self.assertEqual(self.container.as_list(), [self.item0, self.item1])

    def test_as_dict(self):
        """Test converting the DoodadSet to dict.
        """
        self.assertEqual(
            self.container.as_dict(),
            {self.item0.name: self.item0, self.item1.name: self.item1}
        )

    def test_len(self):
        """Test that len(DoodadSet) works.
        """
        self.assertEqual(len(self.container), 2)

    def test_iterator(self):
        """Test that the DoodadSet iterator works.
        """
        self.assertEqual(self.container.as_list(), list(self.container))

    def test_assign(self):
        """Test that we can assign a full list to the Doodad.
        """
        r = [
            challenge.basics.MutableDoodad("c", 4),
            challenge.basics.MutableDoodad("d", 5),
            challenge.basics.MutableDoodad("e", 6)
        ]
        self.container.assign(r)
        self.assertEqual(self.container.as_list(), r)

    def test_tuple_convert(self):
        """Test that we accept a WhatsIt (a Python tuple) in add().
        """
        self.container.add(("g", 13))
        item = self.container.as_dict()["g"]
        self.assertEqual(item.name, "g")
        self.assertEqual(item.value, 13)

    def test_downcast(self):
        """Test that Thingamajigs round-trip through DoodadSet.
        """
        t1 = challenge.extensions.Thingamajig(5.4, "f", 12)
        self.container.add(t1)
        t2 = self.container.as_dict()["f"]
        self.assertEqual(t1, t2)
        self.assertIsInstance(t2, challenge.extensions.Thingamajig)
        self.assertEqual(t1.get_extra(), t2.get_extra())

    def test_class_attr(self):
        """Test that we've defined the C++ Item typedef as a class attribute.
        """
        self.assertEqual(challenge.containers.DoodadSet.Item,
                         challenge.basics.MutableDoodad)

if __name__ == "__main__":
    unittest.main()
