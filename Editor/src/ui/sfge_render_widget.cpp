#include "sfge_render_widget.hpp"

#include <QtGui/QPainter>
#include <QtGui/QBrush>

#include <sfge/graphics/graphic_system.hpp>
#include <sfge/infrastructure/data_store.hpp>

#include <boost/property_tree/json_parser.hpp>

#include <sstream>

#include <SFML/Graphics/View.hpp>

using namespace sfge;
using namespace sf;
using namespace std;
using namespace boost::property_tree;

SFGERenderWidget::SFGERenderWidget(QWidget *parent)
	: QWidget(parent), mSFGEInitialized(false)
{
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_OpaquePaintEvent);
	setAttribute(Qt::WA_NoBackground);

	mRefreshTimer = new QTimer(this);
	mRefreshTimer->setInterval(30);
	connect(mRefreshTimer, SIGNAL(timeout()), this, SLOT(repaint()));

	Game::Init();
}

QPaintEngine* SFGERenderWidget::paintEngine() const
{
	return nullptr;
}

void SFGERenderWidget::InitSFGE()
{
	try
	{
		GraphicSystem::InitParams initParams;
		initParams.mHandle		= winId();
		initParams.mMode.width	= width();
		initParams.mMode.height	= height();

		GraphicSystem::getSingleton().Create(initParams);

		mRefreshTimer->start();
	}
	catch(const exception &e)
	{
		qDebug(e.what());
	}
}

void SFGERenderWidget::paintEvent(QPaintEvent*)
{
	if (!mSFGEInitialized)
	{
		InitSFGE();
		mSFGEInitialized = true;
	}

	Game::ProcessOneFrame();
}

void SFGERenderWidget::resizeEvent(QResizeEvent*)
{
	const float w = static_cast<float>(width()),
				h = static_cast<float>(height());

	RenderWindow &renderWin = GraphicSystem::getSingleton().GetCurrentRenderWindow();
	View view(renderWin.getView());
	view.setSize(w, h);
	renderWin.setView(view);
}
