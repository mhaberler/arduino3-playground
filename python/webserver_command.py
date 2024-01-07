Import("env")

# Single action/command per 1 target
# env.AddCustomTarget("sysenv", None, 'python -c "import os; print(os.environ)"')

# Multiple actions
env.AddCustomTarget(
    name="http_server",
    dependencies=None,
    actions=[
        "python python/GzipSimpleHTTPServer.py --web data/www --port 8000",
    ],
    title="HTTP Server",
    description="Run local HTTP Server"
)
