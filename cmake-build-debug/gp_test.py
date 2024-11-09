from linear_gp import Program, Mutator
import numpy as np

p = Program()

print("Code before mutation")
p.displayCode()

print("Registers after execution")
p.execute(np.array([0.234, 0.1273, 0.4326, 0.911]))
p.displayRegisters()

print("Code after mutation")
Mutator.mutateProgram(p)
p.displayCode()

print("Registers after execution")
p.execute(np.array([0.234, 0.1273, 0.4326, 0.911]))
p.displayRegisters()