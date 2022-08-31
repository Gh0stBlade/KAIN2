#include "CORE.H"
#include "MENU.H"
#include "MENUDEFS.H"
#include "MENUUTIL.H"
#include "DRAW.H"

#include <assert.h>

int menu_data_size()
{ 
	return sizeof(menu_t);
}

void menu_initialize(struct menu_t *menu, void *opaque)
{ 
	memset(menu, 0, sizeof(menu_t));

	menu->nmenus = -1;
	menu->opaque = opaque;
}

void menu_format(struct menu_t *menu, int center, int xpos, int ypos, int width, int lineskip, int itemskip, int border)
{
	struct menu_format_t* fmt;

	fmt = &menu->stack[menu->nmenus].format;
	
	fmt->xpos = xpos;
	fmt->ypos = ypos;
	fmt->center = center;
	fmt->lineskip = lineskip;
	fmt->itemskip = itemskip;
	fmt->width = width;
	fmt->border = border;
}

void menu_set(struct menu_t *menu, int fn(void*, int))
{ 
	menu->nmenus = 0;
	menu->drawfn = NULL;
	menu_push(menu, fn);
}

void menu_push(struct menu_t *menu, int fn(void*, int))
{ 
	struct menu_stack_t* stack;

	stack = &menu->stack[menu->nmenus++];

	(stack + 1)->fn = fn;
	(stack + 1)->index = -1;
	(stack + 1)->format.xpos = stack->format.xpos;
	(stack + 1)->format.ypos = stack->format.ypos;
	(stack + 1)->format.lineskip = stack->format.lineskip;
	(stack + 1)->format.itemskip = stack->format.itemskip;
	(stack + 1)->format.width = stack->format.width;
	(stack + 1)->format.center = stack->format.center;
	(stack + 1)->format.border = stack->format.border;
}

void menu_pop(struct menu_t *menu)
{
	menu->nmenus--;
}

#if defined(_WIN64)
void menu_item_flags(struct menu_t *menu, int (*fn)(void*, long long, enum menu_ctrl_t), long parameter, long flags, char *format, ...)
#else
void menu_item_flags(struct menu_t *menu, int (*fn)(void*, long, enum menu_ctrl_t), long parameter, long flags, char *format, ...)
#endif
{ 
	struct menu_item_t *item;
	va_list ap;

	item = &menu->items[menu->nitems++];
	item->fn = fn;
	item->parameter = parameter;
	item->flags = flags;
	item->text = &menu->bytes[menu->nbytes];
	
	va_start(ap, format);
	
	vsprintf(item->text, format, ap);

	menu->nbytes += strlen(item->text) + 1;
}

#if defined(_WIN64)
void menu_item(struct menu_t *menu, int (*fn)(void*, long long, enum menu_ctrl_t), long long parameter, char* format, ...)
#else
void menu_item(struct menu_t *menu, int (*fn)(void*, long, enum menu_ctrl_t), long parameter, char* format, ...)
#endif
{ 
	struct menu_item_t* item;
	va_list ap;
	
	item = &menu->items[menu->nitems++];
	item->fn = fn;
	item->parameter = parameter;
	item->flags = 0;
	item->text = &menu->bytes[menu->nbytes];

	va_start(ap, format);

	if (format != NULL)
	{
		vsprintf(item->text, format, ap);
		menu->nbytes += strlen(item->text) + 1;
	}
	else
	{
		item->text = NULL;
	}
}

void menu_build(struct menu_t *menu)
{ 
	struct menu_stack_t *stack;
	
	menu->nitems = 0;
	menu->nbytes = 0;

	stack = &menu->stack[menu->nmenus];
	stack->index = stack->fn(menu->opaque, stack->index);
}

void DisplayHintBox(int len, int y)
{
	struct Extents2d ext;
	int i;

	len >>= 1;
	
	ext.xmin = 251 - len;
	ext.xmax = len + 261;
	ext.ymin = (y - 1);
	ext.ymax = y + 13;

	for(i = 0; i < 2; i++)
	{
		DRAW_TranslucentQuad(ext.xmin - 14, (ext.ymin + ext.ymax) >> 1, ext.xmin, ext.ymax, ext.xmin, ext.ymin, ext.xmax, ext.ymax, 0, 0, 0, 0, gameTrackerX.primPool, &gameTrackerX.drawOT[1 * 2]);

		DRAW_TranslucentQuad(ext.xmin, ext.xmax, ext.ymin, ext.ymax, ext.xmax, ext.ymin, ext.xmax + 14, (ext.ymin + ext.ymax) >> 1, 0, 0, 0, 0, gameTrackerX.primPool, &gameTrackerX.drawOT[1 * 2]);
	}
}

void DisplayMenuBox(int x0, int x1, int y0, int y1)
{ 
	struct Extents2d ext;
	static int dy[16] = { 40, 30, 20, 15, 10, 8, 6, 5, 4, 3, 2, 2, 1, 1, 1, 0 };
	int i;
	int j;
	int k;
	int slice0;
	unsigned long** ot;
	struct _PrimPool* primPool;

	slice0 = 0;
	
	x0 -= 12;
	x1 += 12;

	ext.ymin = y0 - 5;
	ext.ymax = y1 + 5;

	primPool = gameTrackerX.primPool;

	y1 -= y0;

	ext.xmin = x0;
	ext.xmax = x1;

	ot = gameTrackerX.drawOT + (1 * 2);

	while (y1 < (dy[slice0] << 1))
	{
		slice0++;
	}

	i = slice0;

	for (j = 0; j < 2; j++)
	{
		k = i + 1;

		for (i = 0; i < 15; i++)
		{
			//Degenerate triangle fix.
#if defined(PSXPC_VERSION)
			DRAW_TranslucentQuad(ext.xmin + i + 1, ext.ymax - dy[i], ext.xmin + k, ext.ymax - dy[k], ext.xmin + i + 1, ext.ymin + dy[i], ext.xmin + k, ext.ymin + dy[k], 0, 0, 0, 0, primPool, ot);

			DRAW_TranslucentQuad(ext.xmax - i + 1, ext.ymax - dy[i], ext.xmax - k, ext.ymax - dy[k], ext.xmax - i + 1, ext.ymin + dy[i], ext.xmax - k, ext.ymin + dy[k], 0, 0, 0, 0, primPool, ot);

#else
			DRAW_TranslucentQuad(ext.xmin + i, ext.ymax - dy[i], ext.xmin + k, ext.ymax - dy[k], ext.xmin + i, ext.ymin + dy[i], ext.xmin + k, ext.ymin + dy[k], 0, 0, 0, 0, primPool, ot);

			DRAW_TranslucentQuad(ext.xmax - i, ext.ymax - dy[i], ext.xmax - k, ext.ymax - dy[k], ext.xmax - i, ext.ymin + dy[i], ext.xmax - k, ext.ymin + dy[k], 0, 0, 0, 0, primPool, ot);
#endif

			i = k;
			k = i + 1;
		}

#if defined(PSXPC_VERSION)
		DRAW_TranslucentQuad(ext.xmin + 15, ext.ymax, ext.xmax - 15 + 1, ext.ymax, ext.xmin + 15, ext.ymin, ext.xmax - 15 + 1, ext.ymin, 0, 0, 0, 0, primPool, ot);
#else
		DRAW_TranslucentQuad(ext.xmin + 15, ext.ymax, ext.xmax - 15, ext.ymax, ext.xmin + 15, ext.ymin, ext.xmax - 15, ext.ymin, 0, 0, 0, 0, primPool, ot);
#endif
		i = slice0;
	}
}

int menu_draw_item(struct menu_t *menu, int ypos, int xadj, int yadj, char *text, int color, long flags, struct Extents2d *e)
{ 
	struct menu_format_t* fmt;
	int numColumns;
	int i;
	int texLen;
	int columnWidth;
	char* columnText;
	char tmpBuff[256];
	char* lineText;
	int columnYPos;
	int maxColumnYPos;
	int center;
	int leftEdge;
	int xpos;
	char* eol;
	char* eop;
	char* tmp;
	int wd;

//Modern strlen/strtok doesn't support NULL being passed in but the old PSX version does.
#if defined(PSXPC_VERSION)
#define strlen(a) a == NULL ? 0 : strlen(a);
#define strtok(a, b) a == NULL ? NULL : strtok(a, b);
#endif
	
	i = menu->nmenus;
	maxColumnYPos = 0;

	fmt = &menu->stack[i].format;

	if (!(flags & 0x1))
	{
		center = 0;

		if (!(flags & 0x2))
		{
			center = fmt->center;
		}
	}
	else
	{
		center = 1;
	}

	if (ypos == 0)
	{
		ypos = fmt->ypos;
	}

	ypos += yadj;
	numColumns = 1;

	texLen = strlen(text);

	if (texLen > 0)
	{
		for (i = 0; i < texLen; i++)
		{
			if (text[i] == 0x9)
			{
				numColumns++;
			}
		}
	}

	columnWidth = fmt->width / numColumns;

	if (fmt->center != 0)
	{
		leftEdge = (fmt->xpos + xadj) - (fmt->width >> 1);
	}
	else
	{
		leftEdge = (fmt->xpos + xadj);
	}

	columnText = strtok(text, "\x9");

	while (columnText != NULL)
	{
		xpos = (leftEdge + xadj) + 0;

		if (center != 0)
		{
			xpos += columnWidth >> 1;
		}

		if (ypos < e->ymin)
		{
			e->ymin = ypos;
		}

		strcpy(tmpBuff, columnText);
		lineText = &tmpBuff[0];

		do
		{
			eol = strchr(lineText, 10);
			if (eol != NULL)
			{
				eol[0] = 0;
			}

			if (lineText != NULL)
			{
				do
				{
					eop = strchr(lineText, 32);

					if (eop != NULL)
					{
						do
						{
							tmp = strchr(eop + 1, 32);
							if (tmp != NULL)
							{
								tmp[0] = 0;
							}

							wd = menu_text_width(lineText);
							if (tmp != NULL)
							{
								tmp[0] = 32;
							}

							if (columnWidth >= wd)
							{
								eop = tmp;
								if (eop == NULL)
								{
									break;
								}
							}
							else
							{
								break;
							}

						} while (1);

						if (eop != NULL)
						{
							eop[0] = 0;
						}
					}

					if (center != 0)
					{
						int s3 = (menu_text_width(lineText) >> 1);
						wd = xpos - s3;
						menu_print(wd, ypos, lineText, color);

						if (wd < e->xmin)
						{
							e->xmin = wd;
						}

						if (e->xmax < xpos + s3)
						{
							e->xmax = xpos + s3;
						}
					}
					else
					{
						wd = menu_text_width(lineText);
						menu_print(xpos, ypos, lineText, color);

						if (xpos < e->xmin)
						{
							e->xmin = xpos;
						}

						if (e->xmax < xpos - wd)
						{
							e->xmax = xpos - wd;
						}
					}

					ypos += fmt->lineskip;

					if (eop == NULL)
					{
						break;
					}

					lineText = eop + 1;

					eop[0] = 32;

				} while (lineText != NULL);
			}

			if (eol == NULL)
			{
				break;
			}

			lineText = eol + 1;
			eol[0] = 10;

		} while (lineText != NULL);

		columnText = strtok(NULL, "\x9");

		if (maxColumnYPos < ypos)
		{
			maxColumnYPos = ypos;
		}
	}

	if (e->ymax < maxColumnYPos)
	{
		e->ymax = maxColumnYPos;
	}

	maxColumnYPos += fmt->itemskip;

	if ((flags & 0x4))
	{
		maxColumnYPos += fmt->lineskip >> 1;
	}

	return maxColumnYPos;
}

void menu_draw(struct menu_t *menu)
{
	struct Extents2d ext;
	struct menu_stack_t* stack;
	int index;
	int ypos;
	int i;
	struct menu_item_t* item;
	int color;

	ext.xmin = 2147483647;
	ext.xmax = 2147483648;
	ext.ymin = 2147483647;
	ext.ymax = 2147483648;

	stack = &menu->stack[menu->nmenus];
	index = stack->index;
	ypos = 0;
	
	if (menu->drawfn != NULL)
	{
		menu->drawfn(menu->opaque);
	}

	for (i = 0; i < menu->nitems; i++)
	{
		item = &menu->items[i];

		if (i != 0)
		{
			color = i != index;
		}
		else
		{
			if (!(item->flags & 0x4))
			{
				color = i != index;
			}
			else
			{
				color = 3;
			}
		}

		ypos = menu_draw_item(menu, ypos, 0, 0, item->text, color, item->flags, &ext);
	}

	if (stack->format.border != 0)
	{
		DisplayMenuBox(ext.xmin, ext.xmax, ext.ymin, ext.ymax);
	}
}

void menu_run(struct menu_t *menu)
{
	enum menu_ctrl_t ctrl;
	struct menu_stack_t *stack;
	int index;
	struct menu_item_t *item;
	enum menu_sound_t sound;
	
	ctrl = menu_get_ctrl(menu->opaque);
	stack = &menu->stack[menu->nmenus];
	index = stack->index;
	item = &menu->items[index];

	if (index >= 0 && ctrl != menu_ctrl_none)
	{
		menudefs_reset_hack_attract_mode();
		sound = (menu_sound_t)item->fn(menu->opaque, item->parameter, ctrl);
		
		if (sound != menu_sound_none)
		{
			menu_sound(sound);
		}
		else
		{
			if (ctrl == menu_ctrl_down)
			{
				index = (index + 1) % menu->nitems;

				if (menu->items[index].fn == NULL)
				{
					do
					{
						index = (index + 1) % menu->nitems;

					} while (menu->items[index].fn == NULL);
				}
			}
			else if (ctrl == menu_ctrl_cancel)
			{
				if (menu->nmenus >= 2)
				{
					menu_sound(menu_sound_pop);
					menu_pop(menu);
				}
			}
			else if (ctrl == menu_ctrl_up)
			{
				index = ((index + menu->nitems - 1) % menu->nitems);
				if (menu->items[index].fn == NULL)
				{
					do
					{
						index = ((index + menu->nitems - 1) % menu->nitems);

					} while (menu->items[index].fn == NULL);
				}
			}

			if (stack->index != index)
			{
				menu_sound(menu_sound_select);
			}

			stack->index = index;
		}
	}
}

void menu_process(struct menu_t *menu)
{ 
	menu_build(menu);
	menu_draw(menu);
	menu_run(menu);
}