"""pybind11 相关的工具"""

def _pybind11_local_repository_impl(ctx):
    """导入 pybind11 bazel 库
    """
    soabi_output = ctx.execute([
        "python3",
        "-c",
        "import sysconfig; print(sysconfig.get_config_var('SOABI'), end='')",
    ])
    ctx.template(
        "build_defs.bzl",
        ctx.attr.build_defs_tpl,
        substitutions = {
            "%{SOABI}": soabi_output.stdout,
        },
    )
    ctx.file("BUILD", content = "")

pybind11_local_repository = repository_rule(
    implementation = _pybind11_local_repository_impl,
    local = True,
    attrs = {"build_defs_tpl": attr.label()},
)
