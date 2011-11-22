Import ('env')

name = 'nuca'
inc = env.Dir('.')
ext_inc = env.Dir ('nuca')
src = []
deps = ['mili', 'biopp']
CXXFLAGS = ['-DMILI_NAMESPACE', '-O3']

env.Append (CXXFLAGS=CXXFLAGS, CFLAGS=CXXFLAGS)
env.CreateHeaderOnlyLibrary(name, inc, ext_inc, deps)
