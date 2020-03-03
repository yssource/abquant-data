# -*- coding: utf-8 -*-

"""Console script for abquant."""
import sys
import click
import json
from abquant.utils.logger import set_loggers
from abquant.utils.logger import system_log as slog
from abquant.data.base import (
    create_stock_day,
    create_stock_min,
    create_stock_xdxr,
    create_all,
)

set_loggers()


@click.group()
@click.version_option()
def cli():
    """Naval Fate.

    This is the docopt example adopted to Click but with some actual
    commands implemented and not just the empty parsing which really
    is not all that interesting.
    """


@cli.group()
def save():
    """Manages ships."""


@save.command("all")
def save_all():
    """Creates a new ship."""
    click.echo("save all")
    create_all()


@cli.group()
def stock():
    """Manages stocks."""


@stock.command("day")
@click.option(
    "ow",
    "--overwrite",
    flag_value=True,
    default=False,
    help="overwrite collection. Default=False.",
)
@click.option(
    "codes", "--add", default="", help="Append stocks into collection. Default=False."
)
def stock_day(ow, codes):
    """stock day."""
    if codes and isinstance(codes, (str,)):
        try:
            codes = json.loads(codes)
        except Exception as e:
            slog.error("--add {} format is invalid.".format(codes))
            slog.error(
                'Please using such a format. --add \'["000001", "300001", "600001"]\'\n'
            )
            return
    create_stock_day(codes)


@stock.command("min")
@click.option(
    "codes", "--add", default="", help="Append stocks into collection. Default=False."
)
@click.option("freqs", "-f", default="", help="1min, 5min, 15min. Default=False.")
def stock_min(codes, freqs):
    """stock min."""
    if codes and isinstance(codes, (str,)):
        try:
            codes = json.loads(codes)
        except Exception as e:
            slog.error("--add {} format is invalid.".format(codes))
            slog.error(
                'Please using such a format. --add \'["000001", "300001", "600001"]\'\n'
            )
            return
    if freqs and isinstance(freqs, (str,)):
        try:
            freqs = json.loads(freqs)
        except Exception as e:
            slog.error("--add {} format is invalid.".format(freqs))
            slog.error(
                'Please using such a format. --add \'["1min", "5min", "30min"]\'\n'
            )
            return
    create_stock_min(codes, freqs)


@stock.command("xdxr")
@click.option(
    "codes", "--add", default="", help="Append stocks xdxr collection. Default=False."
)
def stock_xdxr(codes):
    """stock xdxr."""
    if codes and isinstance(codes, (str,)):
        try:
            codes = json.loads(codes)
        except Exception as e:
            slog.error("--add {} format is invalid.".format(codes))
            slog.error(
                'Please using such a format. --add \'["000001", "300001", "600001"]\'\n'
            )
            return
    create_stock_xdxr(codes)
