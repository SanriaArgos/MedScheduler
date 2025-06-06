export const AppPromoter = () => {
    return <div
        className="mt-10 xl:mt-14 rounded-[30px] flex flex-col xl:flex-row xl:justify-between xl:gap-x-32 overflow-hidden w-full h-fit bg-linear-65 from-main to-main2 min-h-40 px-4 xl:px-10 py-4 xl:py-10">
        <div className="xl:ml-4">
            <h3 className="font-bold text-[24px] xl:text-[32px] text-[white]">Заботимся о вашем времени</h3>
            <p className="mt-4 font-medium text-[16px] xl:text-[18px] text-[white]">
                Наше настольное приложение делает запись к врачу еще удобнее:
            </p>
            <ul className="mt-2 list-disc list-inside text-white text-[16px] xl:text-[18px]">
                <li>Быстрый поиск специалистов</li>
                <li>Управление записями в пару кликов</li>
                <li>И многое другое</li>
            </ul>
        </div>
        <div className="mt-4 xl:mt-0 flex-shrink-0">
            <div className="xl:h-[400px] w-full xl:w-[250px] max-w-full aspect-[9/16] flex relative text-[24px] rounded-[20px] items-center justify-center bg-[white]">
                <div className="w-fit h-fit flex text-gray-400 text-sm">Макет приложения</div>
            </div>
        </div>
    </div>
}

