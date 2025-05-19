#!/usr/bin/python3

from pathlib import Path
from typing import Final
from unittest import TestCase
from doublespace import decompress


COMPRESSED_PATH: Final = Path("tests/compressed.bin")
DECOMPRESSED_PATH: Final = Path("tests/decompressed.bin")


class DecompressTest(TestCase):
    """Tests for the doublespace decompression"""

    def test_argument_count(self) -> None:
        """Test that missing or surplus arguments cause a TypeError exception"""
        self.assertRaises(TypeError, decompress)
        self.assertRaises(TypeError, decompress, bytes())
        self.assertRaises(TypeError, decompress, bytes(), bytearray(), bytes())

    def test_argument_type(self) -> None:
        """Test that invalid arguments cause a TypeError exception"""
        self.assertRaises(TypeError, decompress, 1, bytearray())
        self.assertRaises(TypeError, decompress, bytes(), 1)
        self.assertRaises(TypeError, decompress, bytes(), bytes())

    def test_os_error(self) -> None:
        """Test that a library error results in an OSError exception"""
        with self.assertRaises(OSError) as cm:
            decompress(bytes(), bytearray())

        self.assertIsInstance(cm.exception.args, tuple)
        self.assertIsInstance(cm.exception.args[0], int)
        self.assertIsInstance(cm.exception.args[1], str)

    def test_length_mismatch(self) -> None:
        """Test that a mismatched output length results in an RuntimeError exception"""
        compressed = COMPRESSED_PATH.read_bytes()
        output = bytearray(DECOMPRESSED_PATH.stat().st_size + 1)

        self.assertRaises(RuntimeError, decompress, compressed, output)

    def test_decompression(self) -> None:
        """Test that the doublespace decompression works"""
        decompressed = DECOMPRESSED_PATH.read_bytes()
        compressed = COMPRESSED_PATH.read_bytes()
        output = bytearray(len(decompressed))

        version: int = decompress(compressed, output)

        self.assertEqual(output, decompressed)
        self.assertEqual(version, 1)
