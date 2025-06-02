export const AppPromoter = () => {
    return <div
        className="mt-10 xl:mt-14 rounded-[30px] flex flex-col xl:flex-row xl:justify-between xl:gap-x-32 overflow-hidden w-full h-fit bg-linear-65 from-main to-main2 min-h-40 px-4 xl:px-10 py-4 xl:py-10">
        <div className="xl:ml-4">
            <h3 className="font-bold text-[24px] xl:text-[32px] text-[white]">У нас есть приложение</h3>
            <p className="mt-4 font-medium text-[16px] xl:text-[18px] text-[white]">сюда надо какой-то текст(на выбор - 4 строчки текста/расписанный по 1-2 строчки нумерованный список из 4-5 тезисов</p>
        </div>
        <div className="mt-4 xl:mt-0">
            <div className="xl:h-[500px] w-full xl:w-[unset] max-w-full aspect-[4/3] flex relative text-[24px] rounded-[20px] items-center justify-center bg-[white]">
                <div className="w-fit h-fit flex">якобы скриншот</div>
            </div>
        </div>
    </div>
}