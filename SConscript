Import ('env')

name = 'nuca'
inc = env.Dir('.')
ext_inc = env.Dir ('nuca')
src = []
deps = ['mili', 'biopp']

env.AppendUnique (CPPFLAGS = ['-DMILI_NAMESPACE'])
env.CreateHeaderOnlyLibrary(name, inc, ext_inc, deps)
