#!/usr/bin/env python

import unittest
import challenge.basics

class DoodadTestCase(unittest.TestCase):
    """Test case for for Doodad bindings.

    All tests also require public data members to be wrapped with at least
    getter properties, and most require the standard constructor to be
    wrapped to support at least positional arguments.
    """

    def test_standard_ctor_pos_all(self):
        """Test standard constructor with all positional arguments.
        """
        d = challenge.basics.Doodad("a", 0)
        self.assertIsInstance(d, challenge.basics.Doodad)
        self.assertEqual(d.name, "a")
        self.assertEqual(d.value, 0)

    def test_standard_ctor_pos_default(self):
        """Test standard constructor with positional arguments, using the
        default for the second.
        """
        d = challenge.basics.Doodad("b")
        self.assertIsInstance(d, challenge.basics.Doodad)
        self.assertEqual(d.name, "b")
        self.assertEqual(d.value, 1)

    def test_standard_ctor_kwarg_al(self):
        """Test standard constructor with all keyword arguments.
        """
        d = challenge.basics.Doodad(name="c", value=2)
        self.assertIsInstance(d, challenge.basics.Doodad)
        self.assertEqual(d.name, "c")
        self.assertEqual(d.value, 2)

    def test_standard_ctor_kwarg_default(self):
        """Test standard constructor with keyword arguments, using the
        default for the second.
        """
        d = challenge.basics.Doodad(name="d")
        self.assertIsInstance(d, challenge.basics.Doodad)
        self.assertEqual(d.name, "d")
        self.assertEqual(d.value, 1)

    def test_standard_ctor_mixed(self):
        """Test standard constructor with a combination of positional and
        keyword arguments.
        """
        d = challenge.basics.Doodad("e", value=3)
        self.assertIsInstance(d, challenge.basics.Doodad)
        self.assertEqual(d.name, "e")
        self.assertEqual(d.value, 3)

    def test_tuple_ctor(self):
        """Test construction from tuple (what we've mapped C++ WhatsIt to).
        """
        w = ("f", 4)
        d = challenge.basics.Doodad(w)
        self.assertIsInstance(d, challenge.basics.Doodad)
        self.assertEqual(d.name, "f")
        self.assertEqual(d.value, 4)

    def test_clone(self):
        """Test calling the clone() method, including checking for unnecessary
        copies.
        """
        d1 = challenge.basics.Doodad("g", 5)
        d2 = d1.clone()
        self.assertEqual(d1.name, d2.name)
        self.assertEqual(d1.value, d2.value)
        self.assertTrue(
            challenge.basics.adjacent(d1.get_secret(), d2.get_secret())
        )

    def test_read(self):
        """Test reading a WhatsIt (tuple) into a Doodad.
        """
        d = challenge.basics.Doodad("h", 6)
        d.read(("i", 7))
        self.assertEqual(d.name, "i")
        self.assertEqual(d.value, 7)

    def test_write(self):
        """Test writing a Doodad into a WhatsIt (tuple).
        """
        d = challenge.basics.Doodad("j", 8)
        self.assertEqual(d.write(), ("j", 8))

if __name__ == "__main__":
    unittest.main()
