# -*- coding: utf-8 -*-

"""Console script for abquant."""
import sys
import click
from abquant.utils.logger import set_loggers
from abquant.utils.logger import user_log as ulog

# from abquant.data.stock import Stock
from abquant.data.base import create_stock_day, create_all

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


@save.command('all')
def save_all():
    """Creates a new ship."""
    click.echo('save all')
    create_all()


@cli.group()
def index():
    """Manages indexs."""


@index.command('day')
@click.argument('name')
def index_day(name):
    """Creates a day index."""
    click.echo('Created index %s' % name)
    # import trepan.api; trepan.api.debug(start_opts={'startup-profile': ['/home/jimmy/.config/trepanpy/profile']})
    ulog.info('Created index %s' % name)
    ulog.debug('Created index %s' % name)
    ulog.warn('Created index %s' % name)
    ulog.error('Created index %s' % name)
    # ulog.exception('Created index %s' % name)


@cli.group()
def stock():
    """Manages stocks."""


@stock.command('day')
@click.option('ow', '--overwrite', flag_value=True,
              default=False,
              help='overwrite collection. Default=False.')
@click.option('ap', '--append', default="",
              help='Append stocks into collection. Default=False.')
def stock_day(ow, ap):
    """stock day."""
    click.echo('overwrite {}, append {}'.format(ow, ap))
    # s = Stock()
    click.echo(create_stock_day())
    if ow:
        ulog.debug(ow)
    if not ow and (ap and isinstance(ap, (str, ))):
        for s in ap.split(","):
            ulog.debug(s)

@stock.command('info')
@click.option('ow', '--overwrite', flag_value=True,
              default=False,
              help='overwrite collection. Default=False.')
@click.option('ap', '--append', default="",
              help='Append stocks into collection. Default=False.')
def stock_info(ow, ap):
    """stock info."""
    click.echo('overwrite {}, append {}'.format(ow, ap))
    # s = Stock()
    click.echo(create_stock_day())
    if ow:
        ulog.debug(ow)
    if not ow and (ap and isinstance(ap, (str, ))):
        for s in ap.split(","):
            ulog.debug(s)


@index.command('move')
@click.argument('index')
@click.argument('x', type=float)
@click.argument('y', type=float)
@click.option('--speed', metavar='KN', default=10,
              help='Speed in knots.')
def index_move(index, x, y, speed):
    """Moves INDEX to the day location X,Y."""
    click.echo('Moving index %s to %s,%s with speed %s' % (index, x, y, speed))


@index.command('shoot')
@click.argument('index')
@click.argument('x', type=float)
@click.argument('y', type=float)
def index_shoot(index, x, y):
    """Makes INDEX fire to X,Y."""
    click.echo('Index %s fires to %s,%s' % (index, x, y))


@cli.group('mine')
def mine():
    """Manages mines."""


@mine.command('set')
@click.argument('x', type=float)
@click.argument('y', type=float)
@click.option('ty', '--moored', flag_value='moored',
              default=True,
              help='Moored (anchored) mine. Default.')
@click.option('ty', '--drifting', flag_value='drifting',
              help='Drifting mine.')
def mine_set(x, y, ty):
    """Sets a mine at a specific coordinate."""
    click.echo('Set %s mine at %s,%s' % (ty, x, y))


@mine.command('remove')
@click.argument('x', type=float)
@click.argument('y', type=float)
def mine_remove(x, y):
    """Removes a mine at a specific coordinate."""
    click.echo('Removed mine at %s,%s' % (x, y))


# @click.command()
# def main(args=None):
#     print('xxxxxx')
#     """Console script for abquant."""
#     click.echo("Replace this message by putting your code into "
#                "abquant.cli.main")
#     click.echo("See click documentation at http://click.pocoo.org/")
#     return 0


# if __name__ == "__main__":
#     sys.exit(main())  # pragma: no cover
