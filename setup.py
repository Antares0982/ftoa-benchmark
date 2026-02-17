from setuptools import setup
from setuptools_rust import Binding, RustExtension

setup(
    name="zmij-playground",
    version="0.1.0",
    rust_extensions=[RustExtension("zmij_playground", binding=Binding.PyO3)],
    zip_safe=False,
    python_requires=">=3.7",
)
